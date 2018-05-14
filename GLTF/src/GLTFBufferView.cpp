#include <GLTF_Utils.h>
#include "GLTFBufferView.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "dgn/gltf/WebglConstants.hpp"

GLTF::BufferView::BufferView(int byteOffset, int byteLength, std::shared_ptr<GLTF::Buffer> buffer){
	this->buffer = buffer;
	this->byteOffset = byteOffset;
	this->byteLength = byteLength;
}

GLTF::BufferView::BufferView(std::vector<unsigned char> *data, int dataLength){
	this->buffer = std::shared_ptr<GLTF::Buffer>(new Buffer(data, dataLength));
	this->byteOffset = 0;
	this->byteLength = dataLength;
}

GLTF::BufferView::BufferView(std::vector<uint8_t> *data, int dataLength, GLTF::Constants::WebGL target) : GLTF::BufferView::BufferView(data, dataLength) {
	this->target = target;
}

std::string GLTF::BufferView::typeName() {
	return "bufferView";
}

void GLTF::BufferView::writeJSON(void* writer, GLTF::Options* options) {
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	if (this->buffer) {
		jsonWriter->Key("buffer");
		if (options->version == "1.0") {
			jsonWriter->String(buffer->getStringId().c_str());
		}
		else {
			jsonWriter->Int(this->buffer->id);
		}
	}
	jsonWriter->Key("byteOffset");
	jsonWriter->Int(this->byteOffset);
	jsonWriter->Key("byteLength");
	jsonWriter->Int(this->byteLength);
	if (byteStride != 0 && options->version != "1.0") {
		jsonWriter->Key("byteStride");
		jsonWriter->Int(this->byteStride);
	}
	if ((int)target > 0) {
		jsonWriter->Key("target");
		jsonWriter->Int((int)this->target);
	}
}


///-------------------------------------------------------------------------------

/**Описать область буфера (по умолчанию непрерывную) */
std::shared_ptr<dgn::gltf::GltfBufferView> create(int32_t byteOffset, int32_t byteLength, const std::shared_ptr<dgn::gltf::GltfBuffer> & buffer){
	return std::shared_ptr<dgn::gltf::GltfBufferView>(
		new GLTF::BufferView(byteOffset, byteLength, std::dynamic_pointer_cast<GLTF::Buffer>(buffer))
	);
}

/**Сдвиг от начала буфера к началу области */
int32_t GLTF::BufferView::getByteOffset(){
	return this->byteOffset;
}

void GLTF::BufferView::setByteOffset(int32_t offset){
	this->byteOffset = offset;
}

/**Сдвиг между компонентами. 0 - значения идут подряд */
int32_t GLTF::BufferView::getByteStride(){
	return this->byteStride;
}

void GLTF::BufferView::setByteStride(int32_t stride){
	this->byteStride = stride;
}

/**Размер области в байтах */
int32_t GLTF::BufferView::getByteLength(){
	return this->byteLength;
}

void GLTF::BufferView::setByteLength(int32_t len){
	this->byteLength = len;
}

/**Буфер из которого читаются данные */
std::shared_ptr<dgn::gltf::GltfBuffer> GLTF::BufferView::getBuffer(){
	return this->buffer;
}

void GLTF::BufferView::setBuffer(const std::shared_ptr<dgn::gltf::GltfBuffer> & b){
	this->buffer = std::dynamic_pointer_cast<GLTF::Buffer>(b);
}

/**Какая структура WebGL описывается в этой области данных */
dgn::gltf::WebglConstants GLTF::BufferView::getTarget(){
	return GLTF::Utils::webglConstGLTF2Java(this->target);
}

void GLTF::BufferView::setTarget(dgn::gltf::WebglConstants webConst){
	this->target = GLTF::Utils::webglJava2GLTF(webConst);
}