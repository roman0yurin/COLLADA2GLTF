/**
 * Утилиты для конвертации данных в glTF формат
 * @author Юрин Роман @date 07.05.2018
 **/

#include "Utils.h"
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;

/**
* Выгрузить набор 3D данных в glTF формат согласно опциям
**/
void glTF::Utils::writeAssetToGlTF(GLTF::Asset* asset, COLLADA2GLTF::Options* options, path *outputPath){
	// Create the output directory if it does not exist
	path outputDirectory = outputPath->parent_path();
	if (!std::experimental::filesystem::exists(outputDirectory)) {
		std::experimental::filesystem::create_directories(outputDirectory);
	}

	asset->mergeAnimations();
	asset->removeUnusedNodes(options);
	asset->removeUnusedSemantics();

	if (options->dracoCompression) {
		asset->removeUncompressedBufferViews();
		asset->compressPrimitives(options);
	}

	GLTF::Buffer* buffer = asset->packAccessors();
	if (options->binary && options->version == "1.0") {
		buffer->stringId = "binary_glTF";
	}

	// Create image bufferViews for binary glTF
	if (options->binary && options->embeddedTextures) {
		size_t imageBufferLength = 0;
		std::vector<GLTF::Image*> images = asset->getAllImages();
		for (GLTF::Image* image : images) {
			imageBufferLength += image->byteLength;
		}
		unsigned char* bufferData = buffer->data;
		bufferData = (unsigned char*)realloc(bufferData, buffer->byteLength + imageBufferLength);
		size_t byteOffset = buffer->byteLength;
		for (GLTF::Image* image : images) {
			GLTF::BufferView* bufferView = new GLTF::BufferView(byteOffset, image->byteLength, buffer);
			image->bufferView = bufferView;
			std::memcpy(bufferData + byteOffset, image->data, image->byteLength);
			byteOffset += image->byteLength;
		}
		buffer->data = bufferData;
		buffer->byteLength += imageBufferLength;
	}

	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(s);
	jsonWriter.StartObject();
	asset->writeJSON(&jsonWriter, options);
	jsonWriter.EndObject();

	if (!options->embeddedTextures) {
		for (GLTF::Image* image : asset->getAllImages()) {
			path uri = outputDirectory / image->uri;
			FILE* file = fopen(uri.generic_string().c_str(), "wb");
			if (file != NULL) {
				fwrite(image->data, sizeof(unsigned char), image->byteLength, file);
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
			fwrite(buffer->data, sizeof(unsigned char), buffer->byteLength, file);
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

		std::ofstream file(options->outputPath);
		if (file.is_open()) {
			file << buffer.GetString() << std::endl;
			file.close();
		}
		else {
			std::cout << "ERROR: couldn't write glTF to path '" << options->outputPath << "'" << std::endl;
		}
	} else {
		FILE* file = fopen(outputPath->generic_string().c_str(), "wb");
		if (file != NULL) {
			fwrite("glTF", sizeof(char), 4, file); // magic

			uint32_t* writeHeader = new uint32_t[2];
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
			fwrite(writeHeader, sizeof(uint32_t), 2, file); // GLB header

			writeHeader[0] = jsonString.length() + jsonPadding; // 2.0 - chunkLength / 1.0 - contentLength
			if (options->version == "1.0") {
				writeHeader[1] = 0; // 1.0 - contentFormat
			}
			else {
				writeHeader[1] = 0x4E4F534A; // 2.0 - chunkType JSON
			}
			fwrite(writeHeader, sizeof(uint32_t), 2, file);
			fwrite(jsonString.c_str(), sizeof(char), jsonString.length(), file);
			for (int i = 0; i < jsonPadding; i++) {
				fwrite(" ", sizeof(char), 1, file);
			}
			if (options->version != "1.0") {
				writeHeader[0] = buffer->byteLength + binPadding; // chunkLength
				writeHeader[1] = 0x004E4942; // chunkType BIN
				fwrite(writeHeader, sizeof(uint32_t), 2, file);
			}
			fwrite(buffer->data, sizeof(unsigned char), buffer->byteLength, file);
			for (int i = 0; i < binPadding; i++) {
				fwrite("\0", sizeof(char), 1, file);
			}

			fclose(file);
		}
		else {
			std::cout << "ERROR couldn't write binary glTF to path '" << outputPath << "'" << std::endl;
		}
	}
}