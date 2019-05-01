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
		if(primitive->indices != NULL && primitive->indices->bufferView != NULL) {
			jsonWriter->StartObject();
			primitive->writeJSON(jsonWriter, options);
			jsonWriter->EndObject();
		}
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


//---------------------------------------------------------------------------------------------

/**Новый экземпляр графического объекта */
std::shared_ptr<dgn::gltf::GltfMesh> dgn::gltf::GltfMesh::create(const std::vector<std::shared_ptr<GltfPrimitive>> & primitives){
	auto const mesh = std::shared_ptr<dgn::gltf::GltfMesh>(
		new GLTF::Mesh()
	);
	mesh->setPrimitives(primitives);
	return mesh;
}

/**Набор графических примитивов, составляющих данный объект */
std::vector<std::shared_ptr<dgn::gltf::GltfPrimitive>> GLTF::Mesh::getPrimitives(){
	return std::vector<std::shared_ptr<dgn::gltf::GltfPrimitive>>(this->primitives.begin(), this->primitives.end());
}

void GLTF::Mesh::setPrimitives(const std::vector<std::shared_ptr<dgn::gltf::GltfPrimitive>> & primitives){
	this->primitives.clear();
	for(auto const p : primitives)
		this->primitives.push_back(std::dynamic_pointer_cast<GLTF::Primitive>(p));
}