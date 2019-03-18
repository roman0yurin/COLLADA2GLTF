/**
 * Утилиты для конвертации данных в glTF формат
 * @author Юрин Роман @date 07.05.2018
 **/

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include "GLTF_Utils.h"
#include "COLLADA2GLTFWriter.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "dgn/gltf/JavaOutputSream.hpp"
#include "dgn/gltf/GltfAttributeType.hpp"

#include "ahoy/ahoy.h"
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;

/**
* Выгрузить набор 3D данных в glTF формат согласно опциям
**/
void GLTF::Utils::writeAssetToGlTF(std::shared_ptr<GLTF::Asset> asset, COLLADA2GLTF::Options* options, const std::shared_ptr<dgn::gltf::JavaOutputSream> & output){
	path outputPath = path(options->outputPath);
	// Create the output directory if it does not exist
	path outputDirectory = outputPath.parent_path();
	if (!std::experimental::filesystem::exists(outputDirectory)) {
		std::experimental::filesystem::create_directories(outputDirectory);
	}

	asset->mergeAnimations();
	asset->removeUnusedNodes(options);
	asset->removeUnusedSemantics();

	if (options->dracoCompression) {
		asset->requireExtension(COLLADA2GLTF::DRACO_EXTENSION);
		for (auto const &primitive : asset->getAllPrimitives()){
			std::map<std::string, std::vector<float>> attribToCompress;
			for(auto const &[atrType, accessor] : primitive->attributes)
				accessor->insertFloatData(attribToCompress[atrType]);


			std::vector<unsigned int> indexes;
			primitive->indices->insertIntData(indexes);
			COLLADA2GLTF::Writer::addAttributesToDracoMesh(primitive, attribToCompress, indexes);
		}


		asset->removeUncompressedBufferViews();
		asset->compressPrimitives(options);
	}

	std::shared_ptr<GLTF::Buffer> buffer = asset->packAccessors();
	if (options->binary && options->version == "1.0") {
		buffer->stringId = "binary_glTF";
	}

	// Create image bufferViews for binary glTF
	if (options->binary && options->embeddedTextures) {
		size_t imageBufferLength = 0;
		std::vector<std::shared_ptr<GLTF::Image>> images = asset->getAllImages();
		for (auto const image : images) {
			imageBufferLength += image->byteLength;
		}
		buffer->data->resize(buffer->byteLength + imageBufferLength, 0);
		size_t byteOffset = buffer->byteLength;
		for (auto const image : images) {
			std::shared_ptr<GLTF::BufferView> bufferView(new GLTF::BufferView(byteOffset, image->byteLength, buffer));
			image->bufferView = bufferView;
			std::memcpy(buffer->data->data() + byteOffset, image->data->data(), image->byteLength);
			byteOffset += image->byteLength;
		}
		buffer->byteLength += imageBufferLength;
	}

	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(s);
	jsonWriter.StartObject();
	asset->writeJSON(&jsonWriter, options);
	jsonWriter.EndObject();

	if (!options->embeddedTextures) {
		for (auto const image : asset->getAllImages()) {
			path uri = outputDirectory / image->uri;
			FILE* file = fopen(uri.generic_string().c_str(), "wb");
			if (file != NULL) {
				fwrite(image->data->data(), sizeof(unsigned char), image->byteLength, file);
				fclose(file);
			}
			else {
				std::cout << "ERROR: Couldn't write image to path '" << uri << "'" << std::endl;
			}
		}
	}

	if (!options->embeddedBuffers) {
		path uri = outputDirectory / buffer->uri;
		FILE* file = fopen(uri.generic_string().c_str(), "wb");
		if (file != NULL) {
			fwrite(buffer->data->data(), sizeof(unsigned char), buffer->byteLength, file);
			fclose(file);
		}
		else {
			std::cout << "ERROR: Couldn't write buffer to path '" << uri << "'" << std::endl;
		}
	}

	if (!options->embeddedShaders) {
		for (GLTF::Shader* shader : asset->getAllShaders()) {
			path uri = outputDirectory / shader->uri;
			FILE* file = fopen(uri.generic_string().c_str(), "wb");
			if (file != NULL) {
				fwrite(shader->source.c_str(), sizeof(unsigned char), shader->source.length(), file);
				fclose(file);
			}
			else {
				std::cout << "ERROR: Couldn't write shader to path '" << uri << "'" << std::endl;
			}
		}
	}

	std::string jsonString = s.GetString();
	if (!options->binary) {
		rapidjson::Document jsonDocument;
		jsonDocument.Parse(jsonString.c_str());

		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		jsonDocument.Accept(writer);

		if(output) {
			const char *data = buffer.GetString();
			output->write(std::vector<uint8_t >(data, data + strlen(data)));
		}else{
			std::ofstream file(options->outputPath);
			if (file.is_open()) {
				file << buffer.GetString() << std::endl;
				file.close();
			} else {
				std::cout << "ERROR: couldn't write glTF to path '" << options->outputPath << "'" << std::endl;
			}
		}
	} else {
		/**Фнукция принимает указатель на массив данных, размер элемента, количество элементов**/
		std::function<void(const void *, long unsigned int, int)> writer;
		std::function<void()> endWork = [](){};
		//Способ записи выбираем в зависимости от полученных параметров и абстрагируем его в виде функции.
		if(output){
			writer = [output](const void *data, long unsigned int itemSz, int items){
				std::vector<uint8_t> vec_data((uint8_t *)data, (uint8_t *)data + (itemSz * items));
				output->write(vec_data);
			};
		}else{
			FILE* file = fopen(outputPath.generic_string().c_str(), "wb");
			assert(file != NULL);
			std::function<void()> endWork = [file](){fclose(file);};
			using namespace std::placeholders;
			writer = std::bind(fwrite, _1, _2, _3, file);
		}

		writer("glTF", sizeof(char), 4); // magic

		uint32_t writeHeader[2];
		// version
		if (options->version == "1.0") {
			writeHeader[0] = 1;
		}
		else {
			writeHeader[0] = 2;
		}

		int jsonPadding = (4 - (jsonString.length() & 3)) & 3;
		int binPadding = (4 - (buffer->byteLength & 3)) & 3;

		writeHeader[1] = HEADER_LENGTH + (CHUNK_HEADER_LENGTH + jsonString.length() + jsonPadding + buffer->byteLength + binPadding); // length
		if (options->version != "1.0") {
			writeHeader[1] += CHUNK_HEADER_LENGTH;
		}
		writer(writeHeader, sizeof(uint32_t), 2); // GLB header

		writeHeader[0] = jsonString.length() + jsonPadding; // 2.0 - chunkLength / 1.0 - contentLength
		if (options->version == "1.0") {
			writeHeader[1] = 0; // 1.0 - contentFormat
		}
		else {
			writeHeader[1] = 0x4E4F534A; // 2.0 - chunkType JSON
		}
		writer(writeHeader, sizeof(uint32_t), 2);
		writer(jsonString.c_str(), sizeof(char), jsonString.length());
		for (int i = 0; i < jsonPadding; i++) {
			writer(" ", sizeof(char), 1);
		}
		if (options->version != "1.0") {
			writeHeader[0] = buffer->byteLength + binPadding; // chunkLength
			writeHeader[1] = 0x004E4942; // chunkType BIN
			writer(writeHeader, sizeof(uint32_t), 2);
		}
		writer(buffer->data->data(), sizeof(unsigned char), buffer->byteLength);
		for (int i = 0; i < binPadding; i++) {
			writer("\0", sizeof(char), 1);
		}
		endWork();

	}
}


///----------------------------------------------------------------------------------------------------------------------

/**записать набор 3д данных в glTF согласно опциям */
void dgn::gltf::GltfUtils::writeAssetToGlTF(const std::shared_ptr<dgn::gltf::GltfAsset> & asset, const std::shared_ptr<dgn::gltf::GltfOptions> & options, const std::shared_ptr<JavaOutputSream> & output){
	GLTF::Utils::writeAssetToGlTF(std::dynamic_pointer_cast<GLTF::Asset>(asset), dynamic_cast<COLLADA2GLTF::Options*>(options.get()), output);
}

