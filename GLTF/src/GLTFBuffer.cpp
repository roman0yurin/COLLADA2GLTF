#include "GLTFBuffer.h"
#include "Base64.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/**Создать новый буфер с непосредственным хранением данных */
static std::shared_ptr<dgn::gltf::GltfBuffer> create(const std::vector<uint8_t> & data){
	return std::shared_ptr<dgn::gltf::GltfBuffer>(
		new GLTF::Buffer(
				new std::vector(data),
				data.size()
		)
	);
}

GLTF::Buffer::Buffer(std::vector<uint8_t> *data, int dataLength){
	this->data.reset(data);
	this->byteLength = dataLength;
}

std::string GLTF::Buffer::typeName() {
	return "buffer";
}

void GLTF::Buffer::writeJSON(void* writer, GLTF::Options* options) {
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	jsonWriter->Key("byteLength");
	jsonWriter->Int(this->byteLength);
	if (!options->binary || !options->embeddedBuffers) {
		jsonWriter->Key("uri");
		if (options->embeddedBuffers) {
			uri = "data:application/octet-stream;base64," + std::string(Base64::encode(this->data.get(), this->byteLength));
		}
		else {
			uri = options->name + std::to_string(id) + ".bin";
		}
		jsonWriter->String(uri.c_str());
	}
	GLTF::Object::writeJSON(writer, options);
}



/**Данные размещенные непосредственно в файле (а не по ссылке) */
std::optional<std::vector<uint8_t>> GLTF::Buffer::getData(){
	if(data)
		return std::optional(*data.get());
	else
		return std::nullopt;
}

void GLTF::Buffer::setData(const std::optional<std::vector<uint8_t>> & data){
	if(data.has_value())
		this->data.reset(new std::vector<uint8_t>(data.value()));
	else
		this->data.reset();
}

/**Сссылка на данные */
std::optional<std::string> GLTF::Buffer::getUri(){
	if(this->uri.size() > 0)
		return std::optional(this->uri);
	else
		return std::nullopt;
}

void GLTF::Buffer::setUri(const std::optional<std::string> & uri){
	if(uri.has_value())
		this->uri = uri.value();
	else
		this->uri.clear();
}

/**Если данные берутся через ссылку, то нужно указать длину */
int32_t GLTF::Buffer::getLength(){
	return this->byteLength;
}

void GLTF::Buffer::setLength(int32_t len){
	this->byteLength = len;
}