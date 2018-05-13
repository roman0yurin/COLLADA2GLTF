#include "GLTFBufferView.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

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