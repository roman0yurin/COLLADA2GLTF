#include <GLTF_Utils.h>
#include "GLTFPrimitive.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "dgn/gltf/GltfAttributeType.hpp"

std::shared_ptr<GLTF::Object> GLTF::Primitive::clone(std::shared_ptr<GLTF::Object> clone) {
	auto const primitive = std::dynamic_pointer_cast<GLTF::Primitive>(clone);
	if (primitive != NULL) {
		primitive->attributes = std::map<std::string, std::shared_ptr<GLTF::Accessor>>();
		for (const auto& attribute : this->attributes) {
			primitive->attributes.insert(attribute);
		}
		primitive->indices = this->indices;
		primitive->material = this->material;
		primitive->mode = this->mode;
		primitive->targets = std::vector<std::shared_ptr<Target>>();
		for (std::shared_ptr<Target> target : this->targets) {
			primitive->targets.push_back(target);
		}
	}
	Object::clone(clone);
	return primitive;
}

void GLTF::Primitive::writeJSON(void* writer, Options* options) {
	auto* jsonWriter = static_cast<rapidjson::Writer<rapidjson::StringBuffer>*>(writer);
	jsonWriter->Key("attributes");
	jsonWriter->StartObject();
	for (const auto& attribute : this->attributes) {
		jsonWriter->Key(attribute.first.c_str());
		if (options->version == "1.0") {
			jsonWriter->String(attribute.second->getStringId().c_str());
		}
		else {
			jsonWriter->Int(attribute.second->id);
		}
	}
	jsonWriter->EndObject();
	if (this->indices) {
		jsonWriter->Key("indices");
		if (options->version == "1.0") {
			jsonWriter->String(indices->getStringId().c_str());
		}
		else {
			jsonWriter->Int(indices->id);
		}
	}
	jsonWriter->Key("mode");
	jsonWriter->Int(static_cast<int>(this->mode));
	if (this->material) {
		jsonWriter->Key("material");
		if (options->version == "1.0") {
			jsonWriter->String(material->getStringId().c_str());
		}
		else {
			jsonWriter->Int(material->id);
		}
	}
	if (!this->targets.empty()) {
		jsonWriter->Key("targets");
		jsonWriter->StartArray();
		for (std::shared_ptr<Primitive::Target> target : this->targets) {
			target->writeJSON(writer, options);
		}
		jsonWriter->EndArray();
	}
	Object::writeJSON(writer, options);
}

GLTF::Primitive::Target* GLTF::Primitive::Target::clone(Object* clone) {
	Target* target = dynamic_cast<Target*>(clone);
	if (target != nullptr) {
		target->attributes = std::map<std::string, std::shared_ptr<Accessor>>();
		for (const auto& attribute : this->attributes) {
			target->attributes.insert(attribute);
		}
	}
	return target;
}

void GLTF::Primitive::Target::writeJSON(void* writer, Options* options) {
	auto* jsonWriter = static_cast<rapidjson::Writer<rapidjson::StringBuffer>*>(writer);
	jsonWriter->StartObject();
	for (const auto& attribute : this->attributes) {
		jsonWriter->Key(attribute.first.c_str());
		jsonWriter->Int(attribute.second->id);
	}
	jsonWriter->EndObject();
}


//-----------------------------------------------------------------------------------

/**Создать новый экземпляр */
std::shared_ptr<dgn::gltf::GltfPrimitive> dgn::gltf::GltfPrimitive::create(){
	return std::shared_ptr<dgn::gltf::GltfPrimitive>(
		new GLTF::Primitive()
	);
}

/**Доступ к данным для соответствующего атрибута */
std::shared_ptr<dgn::gltf::GltfAccessor> GLTF::Primitive::getAttribute(dgn::gltf::GltfAttributeType tp){
	auto iter =  this->attributes.find(
			GLTF::Utils::attrTypeJava2Gltf(tp)
	);
	return iter != this->attributes.end() ?
		   		iter->second:
		   		std::shared_ptr<dgn::gltf::GltfAccessor>();
}

void GLTF::Primitive::setAttribute(dgn::gltf::GltfAttributeType tp, const std::shared_ptr<dgn::gltf::GltfAccessor> & accessor){
	auto const key = GLTF::Utils::attrTypeJava2Gltf(tp);
	if(accessor){
		this->attributes[key] = std::dynamic_pointer_cast<GLTF::Accessor>(accessor);
	}else{
		this->attributes.erase(key);
	}
}

/**Материал задает графические свойства поверхности */
std::shared_ptr<dgn::gltf::MaterialCommon> GLTF::Primitive::getMaterial(){
	return std::dynamic_pointer_cast<dgn::gltf::MaterialCommon>(this->material);
}

void GLTF::Primitive::setMaterial(const std::shared_ptr<dgn::gltf::MaterialCommon> & m){
	this->material = std::dynamic_pointer_cast<GLTF::Material>(m);
}

/**задет список вершин для построения примитивов */
std::shared_ptr<dgn::gltf::GltfAccessor> GLTF::Primitive::getIndexes(){
	return this->indices;
}

void GLTF::Primitive::setIndexes(const std::shared_ptr<dgn::gltf::GltfAccessor> & idx){
	this->indices = std::dynamic_pointer_cast<GLTF::Accessor>(idx);
}


/**Режим построения примитива из вершинного массива */
dgn::gltf::GlMode GLTF::Primitive::getMode(){
	return GLTF::Utils::primitiveModeGltf2Java(this->mode);
}

void GLTF::Primitive::setMode(dgn::gltf::GlMode m){
	this->mode = GLTF::Utils::primitiveModeJava2Gltf(m);
}
