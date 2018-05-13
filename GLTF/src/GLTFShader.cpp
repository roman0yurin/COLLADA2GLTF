#include "GLTFShader.h"
#include "GLTFOptions.h"
#include "Base64.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <string>

std::string GLTF::Shader::typeName() {
	return "shader";
}

void GLTF::Shader::writeJSON(void* writer, GLTF::Options* options) {
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;

	jsonWriter->Key("type");
	jsonWriter->Int((int)type);
	jsonWriter->Key("uri");
	std::unique_ptr<std::vector<uint8_t>> data(new std::vector<uint8_t >(source.begin(), source.end()));
	if (options->embeddedShaders) {
		uri = "data:text/plain;base64," + std::string(Base64::encode(data.get(), source.length()));
	} else {
		uri = options->name + std::to_string(id) + (type == GLTF::Constants::WebGL::VERTEX_SHADER ? ".vert" : ".frag");
	}
	jsonWriter->String(uri.c_str());
	GLTF::Object::writeJSON(writer, options);
}