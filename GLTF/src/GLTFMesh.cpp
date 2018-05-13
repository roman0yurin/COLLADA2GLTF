#include "GLTFMesh.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

std::string GLTF::Mesh::typeName() {
	return "mesh";
}

std::shared_ptr<GLTF::Object> GLTF::Mesh::clone(std::shared_ptr<GLTF::Object> clone) {
	std::shared_ptr<GLTF::Mesh> mesh = std::dynamic_pointer_cast<GLTF::Mesh>(clone);
	if (mesh != NULL) {
		for (auto const primitive : this->primitives) {
			std::shared_ptr<GLTF::Primitive> clonePrimitive(new GLTF::Primitive());
			primitive->clone(clonePrimitive);
			if (clonePrimitive != NULL) {
				mesh->primitives.push_back(clonePrimitive);
			}
		}
		mesh->weights = this->weights;
	}
	GLTF::Object::clone(clone);
	return mesh;
}

void GLTF::Mesh::writeJSON(void* writer, GLTF::Options* options) {
	auto* jsonWriter = static_cast<rapidjson::Writer<rapidjson::StringBuffer>*>(writer);
	jsonWriter->Key("primitives");
	jsonWriter->StartArray();
	for (auto const primitive : this->primitives) {
		jsonWriter->StartObject();
		primitive->writeJSON(jsonWriter, options);
		jsonWriter->EndObject();
	}
	jsonWriter->EndArray();
	if (!weights.empty()) {
		jsonWriter->Key("weights");
		jsonWriter->StartArray();
		for (float weight: this->weights) {
			jsonWriter->Double(weight);
		}
		jsonWriter->EndArray();
	}
	GLTF::Object::writeJSON(writer, options);
}