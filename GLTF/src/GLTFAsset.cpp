#include "GLTFAsset.h"

#include <algorithm>
#include <functional>
#include <map>
#include <set>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "GLTF_Utils.h"

GLTF::Asset::Asset() {
	metadata.reset(new GLTF::Asset::Metadata());
	globalSampler.reset(new GLTF::Sampler());
}


void GLTF::Asset::Metadata::writeJSON(void* writer, GLTF::Options* options) {
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	if (options->version != "") {
		version = options->version;
	}
	if (options->version == "1.0") {
		jsonWriter->Key("premultipliedAlpha");
		jsonWriter->Bool(true);
		jsonWriter->Key("profile");
		jsonWriter->StartObject();
		jsonWriter->Key("api");
		jsonWriter->String("WebGL");
		jsonWriter->Key("version");
		jsonWriter->String("1.0.2");
		jsonWriter->EndObject();
	}
	if (copyright.length() > 0) {
		jsonWriter->Key("copyright");
		jsonWriter->String(copyright.c_str());
	}
	if (generator.length() > 0) {
		jsonWriter->Key("generator");
		jsonWriter->String(generator.c_str());
	}
	if (version.length() > 0) {
		jsonWriter->Key("version");
		jsonWriter->String(version.c_str());
	}
	GLTF::Object::writeJSON(writer, options);
}

std::shared_ptr<GLTF::Scene> GLTF::Asset::getDefaultScene() { scene;
	if (this->scene < 0) {
		this->scenes.push_back(std::shared_ptr<GLTF::Scene>(new GLTF::Scene()));
		this->scene = 0;
	}

	return this->scenes[this->scene];
}

std::vector<std::shared_ptr<GLTF::Accessor>> GLTF::Asset::getAllAccessors() {
	std::set<std::shared_ptr<GLTF::Accessor>> uniqueAccessors;
	std::vector<std::shared_ptr<GLTF::Accessor>> accessors;
	for (auto const skin : getAllSkins()) {
		auto const inverseBindMatrices = skin->inverseBindMatrices;
		if (inverseBindMatrices != NULL) {
			if (uniqueAccessors.find(inverseBindMatrices) == uniqueAccessors.end()) {
				accessors.push_back(inverseBindMatrices);
				uniqueAccessors.insert(inverseBindMatrices);
			}
		}
	}

	for (auto const primitive : getAllPrimitives()) {
		for (auto const accessor: getAllPrimitiveAccessors(primitive)) {
			if (uniqueAccessors.find(accessor) == uniqueAccessors.end()) {
				accessors.push_back(accessor);
				uniqueAccessors.insert(accessor);
			}
		}
		auto const indicesAccessor = primitive->indices;
		if (indicesAccessor != NULL) {
			if (uniqueAccessors.find(indicesAccessor) == uniqueAccessors.end()) {
				accessors.push_back(indicesAccessor);
				uniqueAccessors.insert(indicesAccessor);
			}
		}
	}

	for (auto const animation : animations) {
		for (auto const channel : animation->channels) {
			auto const sampler = channel->sampler;
			if (uniqueAccessors.find(sampler->input) == uniqueAccessors.end()) {
				accessors.push_back(sampler->input);
				uniqueAccessors.insert(sampler->input);
			}
			if (uniqueAccessors.find(sampler->output) == uniqueAccessors.end()) {
				accessors.push_back(sampler->output);
				uniqueAccessors.insert(sampler->input);
			}
		}
	}
	return accessors;
}

std::vector<std::shared_ptr<GLTF::Node>> GLTF::Asset::getAllNodes() {
	std::vector<std::shared_ptr<GLTF::Node>> nodeStack;
	std::vector<std::shared_ptr<GLTF::Node>> nodes;
	std::set<std::shared_ptr<GLTF::Node>> uniqueNodes;
	for (auto const node : getDefaultScene()->nodes) {
		nodeStack.push_back(node);
	}
	while (nodeStack.size() > 0) {
		std::shared_ptr<GLTF::Node> node = nodeStack.back();
		if (uniqueNodes.find(node) == uniqueNodes.end()) {
			nodes.push_back(node);
			uniqueNodes.insert(node);
		}
		nodeStack.pop_back();
		for (auto const child : node->children) {
			nodeStack.push_back(child);
		}
		std::shared_ptr<GLTF::Skin> skin = node->skin;
		if (skin != NULL) {
			std::shared_ptr<GLTF::Node> skeleton = skin->skeleton.lock();
			if (skeleton != NULL) {
				nodeStack.push_back(skeleton);
			}
			for (std::weak_ptr<GLTF::Node> jointNode : skin->joints) {
				const std::shared_ptr<Node> &ptr = jointNode.lock();
				if(ptr)
					nodeStack.push_back(ptr);
			}
		}
	}
	return nodes;
}

std::vector<std::shared_ptr<GLTF::Mesh>> GLTF::Asset::getAllMeshes() {
	std::vector<std::shared_ptr<GLTF::Mesh>> meshes;
	std::set<std::shared_ptr<GLTF::Mesh>> uniqueMeshes;
	for (auto const node : getAllNodes()) {
		if (node->mesh != NULL) {
			if (uniqueMeshes.find(node->mesh) == uniqueMeshes.end()) {
				meshes.push_back(node->mesh);
				uniqueMeshes.insert(node->mesh);
			}
		}
	}
	return meshes;
}

std::vector<std::shared_ptr<GLTF::Primitive>> GLTF::Asset::getAllPrimitives() {
	std::vector<std::shared_ptr<GLTF::Primitive>> primitives;
	std::set<std::shared_ptr<GLTF::Primitive>> uniquePrimitives;
	for (auto const mesh : getAllMeshes()) {
		for (auto const primitive : mesh->primitives) {
			if (uniquePrimitives.find(primitive) == uniquePrimitives.end()) {
				primitives.push_back(primitive);
				uniquePrimitives.insert(primitive);
			}
		}
	}
	return primitives;
}

std::vector<std::shared_ptr<GLTF::Skin>> GLTF::Asset::getAllSkins() {
	std::vector<std::shared_ptr<GLTF::Skin>> skins;
	std::set<std::shared_ptr<GLTF::Skin>> uniqueSkins;
	for (auto const node : getAllNodes()) {
		auto const skin = node->skin;
		if (skin != NULL) {
			if (uniqueSkins.find(skin) == uniqueSkins.end()) {
				skins.push_back(skin);
				uniqueSkins.insert(skin);
			}
		}
	}
	return skins;
}

std::vector<std::shared_ptr<GLTF::Material>> GLTF::Asset::getAllMaterials() {
	std::vector<std::shared_ptr<GLTF::Material>> materials;
	std::set<std::shared_ptr<GLTF::Material>> uniqueMaterials;
	for (auto const primitive : getAllPrimitives()) {
		auto const material = primitive->material;
		if (material != NULL) {
			if (uniqueMaterials.find(material) == uniqueMaterials.end()) {
				materials.push_back(material);
				uniqueMaterials.insert(material);
			}
		}
	}
	return materials;
}

std::vector<std::shared_ptr<GLTF::Technique>> GLTF::Asset::getAllTechniques() {
	std::vector<std::shared_ptr<GLTF::Technique>> techniques;
	std::set<std::shared_ptr<GLTF::Technique>> uniqueTechniques;
	for (auto const material : getAllMaterials()) {
		auto const technique = material->technique;
		if (technique != NULL) {
			if (uniqueTechniques.find(technique) == uniqueTechniques.end()) {
				techniques.push_back(technique);
				uniqueTechniques.insert(technique);
			}
		}
	}
	return techniques;
}

std::vector<GLTF::Program*> GLTF::Asset::getAllPrograms() {
	std::vector<GLTF::Program*> programs;
	std::set<GLTF::Program*> uniquePrograms;
	for (auto const technique : getAllTechniques()) {
		GLTF::Program* program = technique->program.get();
		if (program != NULL) {
			if (uniquePrograms.find(program) == uniquePrograms.end()) {
				programs.push_back(program);
				uniquePrograms.insert(program);
			}
		}
	}
	return programs;
}
std::vector<GLTF::Shader*> GLTF::Asset::getAllShaders() {
	std::vector<GLTF::Shader*> shaders;
	std::set<GLTF::Shader*> uniqueShaders;
	for (GLTF::Program* program : getAllPrograms()) {
		GLTF::Shader* vertexShader = program->vertexShader.get();
		if (vertexShader != NULL) {
			if (uniqueShaders.find(vertexShader) == uniqueShaders.end()) {
				shaders.push_back(vertexShader);
				uniqueShaders.insert(vertexShader);
			}
		}
		GLTF::Shader* fragmentShader = program->fragmentShader.get();
		if (fragmentShader != NULL) {
			if (uniqueShaders.find(fragmentShader) == uniqueShaders.end()) {
				shaders.push_back(fragmentShader);
				uniqueShaders.insert(fragmentShader);
			}
		}
	}
	return shaders;
}

std::vector<std::shared_ptr<GLTF::Texture>> GLTF::Asset::getAllTextures() {
	std::vector<std::shared_ptr<GLTF::Texture>> textures;
	std::set<std::shared_ptr<GLTF::Texture>> uniqueTextures;
	for (auto const material : getAllMaterials()) {
		if (material->type == GLTF::Material::MATERIAL || material->type == GLTF::Material::MATERIAL_COMMON) {
			auto const values = material->values;
			if (values->ambientTexture != NULL) {
				if (uniqueTextures.find(values->ambientTexture) == uniqueTextures.end()) {
					textures.push_back(values->ambientTexture);
					uniqueTextures.insert(values->ambientTexture);
				}
			}
			if (values->diffuseTexture != NULL) {
				if (uniqueTextures.find(values->diffuseTexture) == uniqueTextures.end()) {
					textures.push_back(values->diffuseTexture);
					uniqueTextures.insert(values->diffuseTexture);
				}
			}
			if (values->emissionTexture != NULL) {
				if (uniqueTextures.find(values->emissionTexture) == uniqueTextures.end()) {
					textures.push_back(values->emissionTexture);
					uniqueTextures.insert(values->emissionTexture);
				}
			}
			if (values->specularTexture != NULL) {
				if (uniqueTextures.find(values->specularTexture) == uniqueTextures.end()) {
					textures.push_back(values->specularTexture);
					uniqueTextures.insert(values->specularTexture);
				}
			}
			if (values->bumpTexture != NULL) {
				if (uniqueTextures.find(values->bumpTexture) == uniqueTextures.end()) {
					textures.push_back(values->bumpTexture);
					uniqueTextures.insert(values->bumpTexture);
				}
			}
		}
		else if (material->type == GLTF::Material::PBR_METALLIC_ROUGHNESS) {
			auto const materialPBR = std::dynamic_pointer_cast<GLTF::MaterialPBR>(material);
			if (materialPBR->metallicRoughness->baseColorTexture != NULL) {
				if (uniqueTextures.find(materialPBR->metallicRoughness->baseColorTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->metallicRoughness->baseColorTexture->texture);
					uniqueTextures.insert(materialPBR->metallicRoughness->baseColorTexture->texture);
				}
			}
			if (materialPBR->metallicRoughness->metallicRoughnessTexture != NULL) {
				if (uniqueTextures.find(materialPBR->metallicRoughness->metallicRoughnessTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->metallicRoughness->metallicRoughnessTexture->texture);
					uniqueTextures.insert(materialPBR->metallicRoughness->metallicRoughnessTexture->texture);
				}
			}
			if (materialPBR->emissiveTexture != NULL) {
				if (uniqueTextures.find(materialPBR->emissiveTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->emissiveTexture->texture);
					uniqueTextures.insert(materialPBR->emissiveTexture->texture);
				}
			}
			if (materialPBR->normalTexture != NULL) {
				if (uniqueTextures.find(materialPBR->normalTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->normalTexture->texture);
					uniqueTextures.insert(materialPBR->normalTexture->texture);
				}
			}
			if (materialPBR->occlusionTexture != NULL) {
				if (uniqueTextures.find(materialPBR->occlusionTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->occlusionTexture->texture);
					uniqueTextures.insert(materialPBR->occlusionTexture->texture);
				}
			}
			if (materialPBR->specularGlossiness->diffuseTexture != NULL) {
				if (uniqueTextures.find(materialPBR->specularGlossiness->diffuseTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->specularGlossiness->diffuseTexture->texture);
					uniqueTextures.insert(materialPBR->specularGlossiness->diffuseTexture->texture);
				}
			}
			if (materialPBR->specularGlossiness->specularGlossinessTexture != NULL) {
				if (uniqueTextures.find(materialPBR->specularGlossiness->specularGlossinessTexture->texture) == uniqueTextures.end()) {
					textures.push_back(materialPBR->specularGlossiness->specularGlossinessTexture->texture);
					uniqueTextures.insert(materialPBR->specularGlossiness->specularGlossinessTexture->texture);
				}
			}
		}
	}
	return textures;
}

std::vector<std::shared_ptr<GLTF::Image>> GLTF::Asset::getAllImages() {
	std::vector<std::shared_ptr<GLTF::Image>> images;
	std::set<std::shared_ptr<GLTF::Image>> uniqueImages;
	for (auto const texture : getAllTextures()) {
		auto const image = texture->source;
		if (image != NULL) {
			if (uniqueImages.find(image) == uniqueImages.end()) {
				images.push_back(image);
				uniqueImages.insert(image);
			}
		}
	}
	return images;
}

std::vector<std::shared_ptr<GLTF::Accessor>> GLTF::Asset::getAllPrimitiveAccessors(std::shared_ptr<GLTF::Primitive> primitive) const
{
	std::vector<std::shared_ptr<GLTF::Accessor>> accessors;

	for (const auto& attribute : primitive->attributes) {
		accessors.emplace_back(attribute.second);
	}
	for (auto const target: primitive->targets) {
		for (const auto attribute : target->attributes) {
			accessors.emplace_back(attribute.second);
		}
	}

	return move(accessors);
}

std::vector<std::shared_ptr<GLTF::BufferView>> GLTF::Asset::getAllCompressedBufferView() {
	std::vector<std::shared_ptr<GLTF::BufferView>> compressedBufferViews;
	std::set<std::shared_ptr<GLTF::BufferView>> uniqueCompressedBufferViews;
	for (auto const primitive : getAllPrimitives()) {
		auto dracoExtensionPtr = primitive->extensions.find("KHR_draco_mesh_compression");
		if (dracoExtensionPtr != primitive->extensions.end()) {
			auto const bufferView = std::dynamic_pointer_cast<GLTF::DracoExtension>(dracoExtensionPtr->second)->bufferView;
			if (uniqueCompressedBufferViews.find(bufferView) == uniqueCompressedBufferViews.end()) {
				compressedBufferViews.push_back(bufferView);
				uniqueCompressedBufferViews.insert(bufferView);
			}
		}
	}
	return compressedBufferViews;
}

void GLTF::Asset::mergeAnimations() {
	if (animations.size() == 0) { return; }

	std::shared_ptr<GLTF::Animation> mergedAnimation(new GLTF::Animation());

	// Merge all animations. In the future, animations should be grouped
	// according to COLLADA <animation_clip/> nodes.
	for (auto const animation : animations) {
		for (auto const channel : animation->channels) {
			mergedAnimation->channels.push_back(channel);
		}
	}

	animations.clear();
	animations.push_back(mergedAnimation);
}

void GLTF::Asset::removeUncompressedBufferViews() {
	for (auto const primitive : getAllPrimitives()) {
		auto dracoExtensionPtr = primitive->extensions.find("KHR_draco_mesh_compression");
		if (dracoExtensionPtr != primitive->extensions.end()) {
			// Currently assume all attributes are compressed in Draco extension.
			for (const auto accessor: getAllPrimitiveAccessors(primitive)) {
				if (accessor->bufferView) {
					accessor->bufferView.reset();
				}
			}
			auto const indicesAccessor = primitive->indices;
			if (indicesAccessor != NULL && indicesAccessor->bufferView) {
				indicesAccessor->bufferView.reset();
			}
		}
	}
}

void GLTF::Asset::removeUnusedSemantics() {
	for (auto const primitive : getAllPrimitives()) {
		std::shared_ptr<GLTF::Material> material = primitive->material;
		if (material != NULL) {
			auto const values = material->values;
			std::map<std::string, std::shared_ptr<GLTF::Accessor>> attributes = primitive->attributes;
			for (const auto attribute : attributes) {
				std::string semantic = attribute.first;
				if (semantic.find("TEXCOORD") != std::string::npos) {
					std::map<std::string, std::shared_ptr<GLTF::Accessor>>::iterator removeTexcoord = primitive->attributes.find(semantic);
					if (semantic == "TEXCOORD_0") {
						if (values->ambientTexture == NULL && values->diffuseTexture == NULL && values->emissionTexture == NULL && 
								values->specularTexture == NULL && values->bumpTexture == NULL) {
							std::map<std::string, std::shared_ptr<GLTF::Accessor>>::iterator removeTexcoord = primitive->attributes.find(semantic);
							primitive->attributes.erase(removeTexcoord);
							removeAttributeFromDracoExtension(primitive, semantic);
						}
					}
					else {
						// Right now we don't support multiple sets of texture coordinates
						primitive->attributes.erase(removeTexcoord);
						removeAttributeFromDracoExtension(primitive, semantic);
					}
				}
			}
		}
	}
}

void GLTF::Asset::removeAttributeFromDracoExtension(std::shared_ptr<GLTF::Primitive> primitive, const std::string &semantic) {
	auto extensionPtr = primitive->extensions.find("KHR_draco_mesh_compression");
	if (extensionPtr != primitive->extensions.end()) {
		auto const dracoExtension = std::dynamic_pointer_cast<GLTF::DracoExtension>(extensionPtr->second);
		auto attPtr = dracoExtension->attributeToId.find(semantic);
		if (attPtr != dracoExtension->attributeToId.end()) {
			const int att_id = attPtr->second;
			// Remove from the extension.
			dracoExtension->attributeToId.erase(attPtr);
			// Remove from draco mesh.
			dracoExtension->dracoMesh->DeleteAttribute(att_id);
		}
	}
}

bool isUnusedNode(std::shared_ptr<GLTF::Node> node, std::set<std::shared_ptr<GLTF::Node>> skinNodes, bool isPbr) {
	if (node->children.size() == 0 && node->mesh == NULL && node->camera == NULL && node->skin == NULL) {
		if (isPbr || node->light == NULL || node->light->type == GLTF::MaterialCommon::Light::AMBIENT) {
			if (std::find(skinNodes.begin(), skinNodes.end(), node) == skinNodes.end()) {
				return true;
			}
		}
	}
	return false;
}

void GLTF::Asset::removeUnusedNodes(GLTF::Options* options) {
	std::vector<std::shared_ptr<GLTF::Node>> nodeStack;
	std::set<std::shared_ptr<GLTF::Node>> skinNodes;
	bool isPbr = !options->glsl && !options->materialsCommon;
	for (auto const skin : getAllSkins()) {
		const std::shared_ptr<Node> &skl = skin->skeleton.lock();
		if (skl != NULL) {
			skinNodes.insert(skl);
		}
		for (std::weak_ptr<GLTF::Node> jointNode : skin->joints) {
			const std::shared_ptr<Node> &ptr = jointNode.lock();
			if(ptr)
				skinNodes.insert(ptr);
		}
	}

	std::shared_ptr<GLTF::Scene> defaultScene = getDefaultScene();
	bool needsPass = true;
	while (needsPass) {
		needsPass = false;
		for (size_t i = 0; i < defaultScene->nodes.size(); i++) {
			std::shared_ptr<GLTF::Node> node = defaultScene->nodes[i];
			if (isUnusedNode(node, skinNodes, isPbr)) {
				// Nodes associated with ambient lights may be optimized out,
				// but we should hang on to the lights so that they are 
				// still written into the shader or common materials object.
				if (node->light != NULL) {
					_ambientLights.push_back(node->light);
				}
				defaultScene->nodes.erase(defaultScene->nodes.begin() + i);
				i--;
			}
			else {
				nodeStack.push_back(node);
			}
		}
		while (nodeStack.size() > 0) {
			std::shared_ptr<GLTF::Node> node = nodeStack.back();
			nodeStack.pop_back();
			for (size_t i = 0; i < node->children.size(); i++) {
				auto const child = node->children[i];
				if (isUnusedNode(child, skinNodes, isPbr)) {
					if (child->light != NULL) {
						_ambientLights.push_back(child->light);
					}
					// this node is extraneous, remove it
					node->children.erase(node->children.begin() + i);
					i--;
					if (node->children.size() == 0) {
						// another pass may be required to clean up the parent
						needsPass = true;
					}
				}
				else {
					nodeStack.push_back(child);
				}
			}
		}
	}
}

std::shared_ptr<GLTF::BufferView> packAccessorsForTargetByteStride(std::vector<std::shared_ptr<GLTF::Accessor>> accessors, GLTF::Constants::WebGL target, size_t byteStride) {
	std::map<std::shared_ptr<GLTF::Accessor>, size_t> byteOffsets;
	size_t byteLength = 0;
	for (auto const accessor : accessors) {
		int componentByteLength = accessor->getComponentByteLength();
		int padding = byteLength % componentByteLength;
		if (padding != 0) {
			byteLength += (componentByteLength - padding);
		}
		byteOffsets[accessor] = byteLength;
		byteLength += componentByteLength * accessor->getNumberOfComponents() * accessor->count;
	}
	std::vector<uint8_t> *bufferData = new std::vector<uint8_t>(byteLength, 0);
	std::shared_ptr<GLTF::BufferView> bufferView(new GLTF::BufferView(bufferData, byteLength, target));
	for (auto const accessor : accessors) {
		size_t byteOffset = byteOffsets[accessor];
	    std::unique_ptr<GLTF::Accessor> packedAccessor(new GLTF::Accessor(accessor->type, accessor->componentType, byteOffset, accessor->count, bufferView));
		int numberOfComponents = accessor->getNumberOfComponents();
		float component[numberOfComponents];
		for (int i = 0; i < accessor->count; i++) {
			accessor->getComponentAtIndex(i, component);
			packedAccessor->writeComponentAtIndex(i, component);
		}
		accessor->byteOffset = packedAccessor->byteOffset;
		accessor->bufferView = packedAccessor->bufferView;
	}
	return bufferView;
}

bool GLTF::Asset::compressPrimitives(GLTF::Options* options) {
	int totalPrimitives = 0;
	for (auto const primitive : getAllPrimitives()) {
		totalPrimitives++;
		auto dracoExtensionPtr = primitive->extensions.find("KHR_draco_mesh_compression");
		if (dracoExtensionPtr == primitive->extensions.end()) {
			// No extension exists.
			continue;
		}
		auto const dracoExtension = std::dynamic_pointer_cast<GLTF::DracoExtension>(dracoExtensionPtr->second);
		draco::Mesh *dracoMesh = dracoExtension->dracoMesh.get();
		if (!dracoMesh) {
			continue;
		}

		// Compress the mesh
		// Setup encoder options.
		draco::Encoder encoder;
		const int posQuantizationBits = options->positionQuantizationBits;
		const int texcoordsQuantizationBits = options->texcoordQuantizationBits;
		const int normalsQuantizationBits = options->normalQuantizationBits;
		const int colorQuantizationBits = options->colorQuantizationBits;
		// Used for compressing joint indices and joint weights.
		const int genericQuantizationBits = options->jointQuantizationBits;

		encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, posQuantizationBits);
		encoder.SetAttributeQuantization(draco::GeometryAttribute::TEX_COORD, texcoordsQuantizationBits);
		encoder.SetAttributeQuantization(draco::GeometryAttribute::NORMAL, normalsQuantizationBits);
		encoder.SetAttributeQuantization(draco::GeometryAttribute::COLOR, colorQuantizationBits);
		encoder.SetAttributeQuantization(draco::GeometryAttribute::GENERIC, genericQuantizationBits);

		draco::EncoderBuffer buffer;
		const draco::Status status = encoder.EncodeMeshToBuffer(*dracoMesh, &buffer);
		if (!status.ok()) {
			std::cerr << "Error: Encode mesh.\n";
			return false;
		}

		// Add compressed data to bufferview
		std::vector<uint8_t> *allocatedData = new std::vector<uint8_t>(buffer.size(), 0);
		std::memcpy(allocatedData->data(), buffer.data(), buffer.size());
		std::shared_ptr<GLTF::BufferView> bufferView(new GLTF::BufferView(allocatedData, buffer.size()));
		dracoExtension->bufferView = bufferView;
		// Remove the mesh so duplicated primitives don't need to compress again.
		dracoExtension->dracoMesh.reset();
	}
	return true;
}

std::shared_ptr<GLTF::Buffer> GLTF::Asset::packAccessors() {
	std::map<GLTF::Constants::WebGL, std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>>> accessorGroups;
	const std::function<std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>> ()> defaultAccessorGroup = [](){
		return std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>>();
	};
	const std::function<std::vector<std::shared_ptr<GLTF::Accessor>> ()> defaultByteStride = [](){
		return std::vector<std::shared_ptr<GLTF::Accessor>>();
	};

	accessorGroups[GLTF::Constants::WebGL::ARRAY_BUFFER] = std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>>();
	accessorGroups[GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER] = std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>>();
	accessorGroups[(GLTF::Constants::WebGL)-1] = std::map<int, std::vector<std::shared_ptr<GLTF::Accessor>>>();

	size_t byteLength = 0;
	for (auto const accessor : getAllAccessors()) {
		// In glTF 2.0, bufferView is not required in accessor.
		if (accessor->bufferView == NULL) {
			continue;
		}
		GLTF::Constants::WebGL target = accessor->bufferView->target;
		auto& targetGroup = GLTF::Utils::withDefault(accessorGroups, target, defaultAccessorGroup);
		int byteStride = accessor->getByteStride();
		std::vector<std::shared_ptr<GLTF::Accessor>> &byteStrideGroup = GLTF::Utils::withDefault(targetGroup, byteStride, defaultByteStride);
		byteStrideGroup.push_back(accessor);
		byteLength += accessor->bufferView->byteLength;
	}

	// Go through primitives and look for primitives that use Draco extension.
	// If extension is not enabled, the vector will be empty.
	std::vector<std::shared_ptr<GLTF::BufferView>> compressedBufferViews = getAllCompressedBufferView();
	// Reserve data for compressed data.
	for (auto const compressedBufferView : compressedBufferViews) {
		byteLength += compressedBufferView->byteLength;
	}

	std::vector<int> byteStrides;
	std::map<int, std::vector<std::shared_ptr<GLTF::BufferView>>> bufferViews;
	const std::function<std::vector<std::shared_ptr<GLTF::BufferView>> (const int)> makeBufferView = [&byteStrides](const int byteStride){
		byteStrides.push_back(byteStride);
		return std::vector<std::shared_ptr<GLTF::BufferView>>();
	};
	for (auto targetGroup : accessorGroups) {
		for (auto byteStrideGroup : targetGroup.second) {
			GLTF::Constants::WebGL target = targetGroup.first;
			int byteStride = byteStrideGroup.first;
			std::shared_ptr<GLTF::BufferView> bufferView = packAccessorsForTargetByteStride(byteStrideGroup.second, target, byteStride);
			if (target == GLTF::Constants::WebGL::ARRAY_BUFFER) {
				bufferView->byteStride = byteStride;
			}
			auto findBufferViews = bufferViews.find(byteStride);
			std::vector<std::shared_ptr<GLTF::BufferView>> &bufferViewGroup = GLTF::Utils::cache(bufferViews, byteStride, makeBufferView);
			bufferViewGroup.push_back(bufferView);
		}
	}
	std::sort(byteStrides.begin(), byteStrides.end(), std::greater<int>());

	// Pack these into a buffer sorted from largest byteStride to smallest
	std::vector<uint8_t > *bufferData = new std::vector<uint8_t >(byteLength, 0);
	std::shared_ptr<GLTF::Buffer> buffer(new GLTF::Buffer(bufferData, byteLength));
	size_t byteOffset = 0;
	for (int byteStride : byteStrides) {
		for (std::shared_ptr<GLTF::BufferView> bufferView : bufferViews[byteStride]) {
			std::memcpy(bufferData->data() + byteOffset, bufferView->buffer->data->data(), bufferView->byteLength);
			bufferView->byteOffset = byteOffset;
			bufferView->buffer = buffer;
			byteOffset += bufferView->byteLength;
		}
	}

	// Append compressed data to buffer.
	for (auto const compressedBufferView : compressedBufferViews) {
		std::memcpy(bufferData->data() + byteOffset, compressedBufferView->buffer->data->data(), compressedBufferView->byteLength);
		compressedBufferView->byteOffset = byteOffset;
		compressedBufferView->buffer = buffer;
		byteOffset += compressedBufferView->byteLength;
	}
	return buffer;
}

void GLTF::Asset::requireExtension(std::string extension) {
	useExtension(extension);
	extensionsRequired.insert(extension);
}

void GLTF::Asset::useExtension(std::string extension) {
	extensionsUsed.insert(extension);
}

void GLTF::Asset::writeJSON(void* writer, GLTF::Options* options) {
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	std::map<std::shared_ptr<GLTF::Image>, std::shared_ptr<GLTF::Texture>> _pbrTextureCache;

	if (options->binary && options->version == "1.0") {
		useExtension("KHR_binary_glTF");
	}

	// Write asset metadata
	if (this->metadata) {
		jsonWriter->Key("asset");
		jsonWriter->StartObject();
		this->metadata->writeJSON(writer, options);
		jsonWriter->EndObject();
	}

	// Write scenes and build node array
	std::vector<std::shared_ptr<GLTF::Node>> nodes;
	if (this->scenes.size() > 0) {
		jsonWriter->Key("scenes");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		int sceneId = 0;
		for (auto const scene : this->scenes) {
			scene->id = sceneId;
			sceneId++;
			std::vector<std::shared_ptr<GLTF::Node>> nodeStack;
			for (auto const node : scene->nodes) {
				assert(node);
				nodeStack.push_back(node);
			}
			while (nodeStack.size() > 0) {
				auto const node = nodeStack.back();
				nodeStack.pop_back();
				if (node->id < 0) {
					node->id = nodes.size();
					nodes.push_back(node);
				}
				for (auto const child : node->children) {
					assert(child);
					nodeStack.push_back(child);
				}
				if (node->skin) {
					std::shared_ptr<GLTF::Skin> skin = node->skin;
					const std::shared_ptr<Node> &skl = skin->skeleton.lock();
					if (skl) {
						nodeStack.push_back(skl);
					}
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(scene->getStringId().c_str());
			}
			jsonWriter->StartObject();
			scene->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}

	// Write scene
	if (this->scene >= 0) {
		jsonWriter->Key("scene");
		if (options->version == "1.0") {
			jsonWriter->String(this->scenes[0]->getStringId().c_str());
		}
		else {
			jsonWriter->Int(this->scene);
		}
	}

	// Write nodes and build mesh, skin, camera, and light arrays
	std::vector<std::shared_ptr<GLTF::Mesh>> meshes;
	std::vector<std::shared_ptr<GLTF::Skin>> skins;
	std::vector<std::shared_ptr<GLTF::Camera>> cameras;
	std::vector<std::shared_ptr<GLTF::MaterialCommon::Light>> lights = _ambientLights;
	if (nodes.size() > 0) {
		jsonWriter->Key("nodes");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (std::shared_ptr<GLTF::Node> node : nodes) {
			std::shared_ptr<GLTF::Mesh> mesh = node->mesh;
			if (mesh != NULL) {
				if (mesh->id < 0) {
					mesh->id = meshes.size();
					meshes.push_back(mesh);
				}
			}
			if (node->skin != NULL) {
				if (node->skin->id < 0) {
					node->skin->id = skins.size();
					skins.push_back(node->skin);
				}
			}
			if (node->camera != NULL) {
				if (node->camera->id < 0) {
					node->camera->id = cameras.size();
					cameras.push_back(node->camera);
				}
			}
			std::shared_ptr<GLTF::MaterialCommon::Light> light = node->light;
			if (light != NULL && light->id < 0) {
				light->id = lights.size();
				lights.push_back(light);
			}
			if (options->version == "1.0") {
				jsonWriter->Key(node->getStringId().c_str());
			}
			jsonWriter->StartObject();
			node->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	nodes.clear();

	// Write cameras
	if (cameras.size() > 0) {
		jsonWriter->Key("cameras");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const camera : cameras) {
			if (options->version == "1.0") {
				jsonWriter->Key(camera->getStringId().c_str());
			}
			jsonWriter->StartObject();
			camera->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}

	// Write meshes and build accessor and material arrays
	std::vector<std::shared_ptr<GLTF::Accessor>> accessors;
	std::vector<std::shared_ptr<GLTF::BufferView>> bufferViews;
	std::vector<std::shared_ptr<GLTF::Material>> materials;
	std::map<std::string, std::shared_ptr<GLTF::Technique>> generatedTechniques;
	if (meshes.size() > 0) {
		jsonWriter->Key("meshes");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const mesh : meshes) {
			for (auto const primitive : mesh->primitives) {
				if (primitive->material && primitive->material->id < 0) {
					std::shared_ptr<GLTF::Material> material = primitive->material;
					if (!options->materialsCommon) {
						if (material->type == GLTF::Material::Type::MATERIAL_COMMON) {
							auto const materialCommon = std::dynamic_pointer_cast<GLTF::MaterialCommon>(material);
							if (options->glsl) {
								std::string techniqueKey = materialCommon->getTechniqueKey(options);
								std::map<std::string, std::shared_ptr<GLTF::Technique>>::iterator findTechnique = generatedTechniques.find(techniqueKey);
								if (findTechnique != generatedTechniques.end()) {
									material = std::shared_ptr<Material>(new GLTF::Material());
									material->name = materialCommon->name;
									material->values = materialCommon->values;
									material->technique = findTechnique->second;
								}
								else {
									bool hasColor = primitive->attributes.find("COLOR_0") != primitive->attributes.end();
									material = materialCommon->getMaterial(lights, hasColor, options);
									generatedTechniques[techniqueKey] = material->technique;
								}
							}
							else {
								std::shared_ptr<GLTF::MaterialPBR> materialPbr = materialCommon->getMaterialPBR(options);
								if (options->lockOcclusionMetallicRoughness && materialPbr->occlusionTexture != NULL) {
									GLTF::MaterialPBR::Texture* metallicRoughnessTexture =new GLTF::MaterialPBR::Texture();
									metallicRoughnessTexture->texture = materialPbr->occlusionTexture->texture;
									materialPbr->metallicRoughness->metallicRoughnessTexture.reset(metallicRoughnessTexture);
								}
								else if (options->metallicRoughnessTexturePaths.size() > 0) {
									std::string metallicRoughnessTexturePath = options->metallicRoughnessTexturePaths[0];
									if (options->metallicRoughnessTexturePaths.size() > 1) {
										size_t index = materials.size();
										if (index < options->metallicRoughnessTexturePaths.size()) {
											metallicRoughnessTexturePath = options->metallicRoughnessTexturePaths[index];
										}
									}
									if (options->metallicRoughnessTexturePaths.size() == 1) {
										metallicRoughnessTexturePath = options->metallicRoughnessTexturePaths[0];
									}
									std::shared_ptr<GLTF::MaterialPBR::Texture> metallicRoughnessTexture(new GLTF::MaterialPBR::Texture());
									const std::shared_ptr<GLTF::Image>& image = GLTF::Image::load(metallicRoughnessTexturePath, this->imageCache);
									std::map<std::shared_ptr<GLTF::Image>, std::shared_ptr<GLTF::Texture>>::iterator textureCacheIt = _pbrTextureCache.find(image);
									std::shared_ptr<GLTF::Texture> texture;
									if (textureCacheIt == _pbrTextureCache.end()) {
										texture.reset(new GLTF::Texture());
										texture->sampler = globalSampler;
										texture->source = image;
										_pbrTextureCache[image] = texture;
									}
									else {
										texture = textureCacheIt->second;
									}
									metallicRoughnessTexture->texture = texture;
									materialPbr->metallicRoughness->metallicRoughnessTexture = metallicRoughnessTexture;
								}
								material = materialPbr;
							}
						}
					}
					primitive->material = material;
					material->id = materials.size();
					materials.push_back(material);
				}

				// Find bufferViews of compressed data. These bufferViews does not belong to Accessors.
				auto dracoExtensionPtr = primitive->extensions.find("KHR_draco_mesh_compression");
					if (dracoExtensionPtr != primitive->extensions.end()) {
						std::shared_ptr<GLTF::BufferView> bufferView = std::dynamic_pointer_cast<GLTF::DracoExtension>(dracoExtensionPtr->second)->bufferView;
						if (bufferView->id < 0) {
							bufferView->id = bufferViews.size();
							bufferViews.push_back(bufferView);
						}
					}

				if (primitive->indices) {
					auto const indices = primitive->indices;
					if (indices->id < 0) {
						indices->id = accessors.size();
						accessors.push_back(indices);
					}
				}
				for (auto const accessor: getAllPrimitiveAccessors(primitive)) {
					if (accessor->id < 0) {
						accessor->id = accessors.size();
						accessors.push_back(accessor);
					}
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(mesh->getStringId().c_str());
			}
			jsonWriter->StartObject();
			mesh->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}

	// Write animations and add accessors to the accessor array
	if (animations.size() > 0) {
		jsonWriter->Key("animations");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (size_t i = 0; i < animations.size(); i++) {
			auto const animation = animations[i];
			animation->id = i;
			int numChannels = 0;
			for (auto const channel : animation->channels) {
				const std::shared_ptr<Node> &nPtr = channel->target->node.lock();
				if ( nPtr && nPtr->id >= 0) {
					numChannels++;
					std::shared_ptr<GLTF::Animation::Sampler> sampler = channel->sampler;
					if (sampler->input->id < 0) {
						sampler->input->id = accessors.size();
						accessors.push_back(sampler->input);
					}
					if (sampler->output->id < 0) {
						sampler->output->id = accessors.size();
						accessors.push_back(sampler->output);
					}
				}
			}
			if (numChannels > 0) {
				if (options->version == "1.0") {
					jsonWriter->Key(animation->getStringId().c_str());
				}
				jsonWriter->StartObject();
				animation->writeJSON(writer, options);
				jsonWriter->EndObject();
			}
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}

	// Write skins and add accessors to the accessor array
	if (skins.size() > 0) {
		jsonWriter->Key("skins");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const skin : skins) {
			if (skin->inverseBindMatrices != NULL && skin->inverseBindMatrices->id < 0) {
				skin->inverseBindMatrices->id = accessors.size();
				accessors.push_back(skin->inverseBindMatrices);
			}
			if (options->version == "1.0") {
				jsonWriter->Key(skin->getStringId().c_str());
			}
			jsonWriter->StartObject();
			skin->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	skins.clear();

	// Write accessors and add bufferViews to the bufferView array
	if (accessors.size() > 0) {
		jsonWriter->Key("accessors");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const accessor : accessors) {
			if (accessor->bufferView) {
				std::shared_ptr<GLTF::BufferView> bufferView = accessor->bufferView;
				if (bufferView->id < 0) {
					bufferView->id = bufferViews.size();
					bufferViews.push_back(bufferView);
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(accessor->getStringId().c_str());
			}
			jsonWriter->StartObject();
			accessor->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}

	if (options->dracoCompression) {
		this->requireExtension("KHR_draco_mesh_compression");
	}

	meshes.clear();
	accessors.clear();

	// Write materials and build technique and texture arrays
	std::vector<std::shared_ptr<GLTF::Technique>> techniques;
	std::vector<std::shared_ptr<GLTF::Texture>> textures;
	bool usesTechniqueWebGL = false;
	bool usesMaterialsCommon = false;
	bool usesSpecularGlossiness = false;
	if (materials.size() > 0) {
		jsonWriter->Key("materials");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const material : materials) {
			if (material->type == GLTF::Material::Type::MATERIAL || material->type == GLTF::Material::Type::MATERIAL_COMMON) {
				if (material->type == GLTF::Material::Type::MATERIAL) {
					std::shared_ptr<GLTF::Technique> technique = material->technique;
					if (technique && technique->id < 0) {
						technique->id = techniques.size();
						techniques.push_back(technique);
					}
					if (!usesTechniqueWebGL) {
						if (options->version != "1.0") {
							this->requireExtension("KHR_technique_webgl");
						}
						usesTechniqueWebGL = true;
					}
				}
				else if (material->type == GLTF::Material::Type::MATERIAL_COMMON && !usesMaterialsCommon) {
					this->requireExtension("KHR_materials_common");
					usesMaterialsCommon = true;
				}
				auto const ambientTexture = material->values->ambientTexture;
				if (ambientTexture != NULL && ambientTexture->id < 0) {
					ambientTexture->id = textures.size();
					textures.push_back(ambientTexture);
				}
				auto const diffuseTexture = material->values->diffuseTexture;
				if (diffuseTexture != NULL && diffuseTexture->id < 0) {
					diffuseTexture->id = textures.size();
					textures.push_back(diffuseTexture);
				}
				auto const emissionTexture = material->values->emissionTexture;
				if (emissionTexture != NULL && emissionTexture->id < 0) {
					emissionTexture->id = textures.size();
					textures.push_back(emissionTexture);
				}
				auto const specularTexture = material->values->specularTexture;
				if (specularTexture != NULL && specularTexture->id < 0) {
					specularTexture->id = textures.size();
					textures.push_back(specularTexture);
				}
			}
			else if (material->type == GLTF::Material::Type::PBR_METALLIC_ROUGHNESS) {
				std::shared_ptr<GLTF::MaterialPBR> materialPBR = std::dynamic_pointer_cast<GLTF::MaterialPBR>(material);
				std::shared_ptr<GLTF::MaterialPBR::Texture> baseColorTexture = materialPBR->metallicRoughness->baseColorTexture;
				if (baseColorTexture != NULL && baseColorTexture->texture->id < 0) {
					baseColorTexture->texture->id = textures.size();
					textures.push_back(baseColorTexture->texture);
				}
				std::shared_ptr<GLTF::MaterialPBR::Texture> metallicRoughnessTexture = materialPBR->metallicRoughness->metallicRoughnessTexture;
				if (metallicRoughnessTexture != NULL && metallicRoughnessTexture->texture->id < 0) {
					metallicRoughnessTexture->texture->id = textures.size();
					textures.push_back(metallicRoughnessTexture->texture);
				}
				std::shared_ptr<GLTF::MaterialPBR::Texture> normalTexture = materialPBR->normalTexture;
				if (normalTexture != NULL && normalTexture->texture->id < 0) {
					normalTexture->texture->id = textures.size();
					textures.push_back(normalTexture->texture);
				}
				std::shared_ptr<GLTF::MaterialPBR::Texture> occlusionTexture = materialPBR->occlusionTexture;
				if (occlusionTexture != NULL && occlusionTexture->texture->id < 0) {
					occlusionTexture->texture->id = textures.size();
					textures.push_back(occlusionTexture->texture);
				}
				std::shared_ptr<GLTF::MaterialPBR::Texture> emissiveTexture = materialPBR->emissiveTexture;
				if (emissiveTexture != NULL && emissiveTexture->texture->id < 0) {
					emissiveTexture->texture->id = textures.size();
					textures.push_back(emissiveTexture->texture);
				}
				if (options->specularGlossiness) {
					if (!usesSpecularGlossiness) {
						this->useExtension("KHR_materials_pbrSpecularGlossiness");
						usesSpecularGlossiness = true;
					}
					std::shared_ptr<GLTF::MaterialPBR::Texture> diffuseTexture = materialPBR->specularGlossiness->diffuseTexture;
					if (diffuseTexture != NULL && diffuseTexture->texture->id < 0) {
						diffuseTexture->texture->id = textures.size();
						textures.push_back(diffuseTexture->texture);
					}
					std::shared_ptr<GLTF::MaterialPBR::Texture> specularGlossinessTexture = materialPBR->specularGlossiness->specularGlossinessTexture;
					if (specularGlossinessTexture != NULL && specularGlossinessTexture->texture->id < 0) {
						specularGlossinessTexture->texture->id = textures.size();
						textures.push_back(specularGlossinessTexture->texture);
					}
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(material->getStringId().c_str());
			}
			jsonWriter->StartObject();
			material->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	materials.clear();

	// Write lights
	if (usesMaterialsCommon && lights.size() > 0) {
		jsonWriter->Key("extensions");
		jsonWriter->StartObject();
		jsonWriter->Key("KHR_materials_common");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (std::shared_ptr<GLTF::MaterialCommon::Light> light : lights) {
			if (options->version == "1.0") {
				jsonWriter->Key(light->getStringId().c_str());
			}
			jsonWriter->StartObject();
			light->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		lights.clear();
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
		jsonWriter->EndObject();
	}

	// Write textures and build sampler and image arrays
	std::vector<std::shared_ptr<GLTF::Sampler>> samplers;
	std::vector<std::shared_ptr<GLTF::Image>> images;
	if (textures.size() > 0) {
		jsonWriter->Key("textures");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const texture : textures) {
			auto const sampler = texture->sampler;
			if (sampler && sampler->id < 0) {
				sampler->id = samplers.size();
				samplers.push_back(sampler);
			}
			auto const source = texture->source;
			if (source && source->id < 0) {
				source->id = images.size();
				images.push_back(source);
			}
			if (options->version == "1.0") {
				jsonWriter->Key(texture->getStringId().c_str());
			}
			jsonWriter->StartObject();
			texture->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	textures.clear();

	// Write images and add bufferViews if we have them
	if (images.size() > 0) {
		jsonWriter->Key("images");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const image : images) {
			std::shared_ptr<GLTF::BufferView> bufferView = image->bufferView;
			if (bufferView != NULL && bufferView->id < 0) {
				bufferView->id = bufferViews.size();
				bufferViews.push_back(bufferView);
			}
			if (options->version == "1.0") {
				jsonWriter->Key(image->getStringId().c_str());
			}
			jsonWriter->StartObject();
			image->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	images.clear();

	// Write samplers
	if (samplers.size() > 0) {
		jsonWriter->Key("samplers");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const sampler : samplers) {
			if (options->version == "1.0") {
				jsonWriter->Key(sampler->getStringId().c_str());
			}
			jsonWriter->StartObject();
			sampler->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	samplers.clear();

	// Write techniques and build program array
	std::vector<GLTF::Program*> programs;
	if (techniques.size() > 0) {
		jsonWriter->Key("techniques");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (std::shared_ptr<GLTF::Technique> technique : techniques) {
			if (technique->program) {
				GLTF::Program* program = technique->program.get();
				if (program->id < 0) {
					program->id = programs.size();
					programs.push_back(program);
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(technique->getStringId().c_str());
			}
			jsonWriter->StartObject();
			technique->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	techniques.clear();

	// Write programs and build shader array
	std::vector<GLTF::Shader*> shaders;
	if (programs.size() > 0) {
		jsonWriter->Key("programs");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (GLTF::Program* program : programs) {
			GLTF::Shader* vertexShader = program->vertexShader.get();
			if (vertexShader != NULL && vertexShader->id < 0) {
				vertexShader->id = shaders.size();
				shaders.push_back(vertexShader);
			}
			GLTF::Shader* fragmentShader = program->fragmentShader.get();
			if (fragmentShader != NULL && fragmentShader->id < 0) {
				fragmentShader->id = shaders.size();
				shaders.push_back(fragmentShader);
			}
			if (options->version == "1.0") {
				jsonWriter->Key(program->getStringId().c_str());
			}
			jsonWriter->StartObject();
			program->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	programs.clear();

	// Write shaders
	if (shaders.size() > 0) {
		jsonWriter->Key("shaders");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (GLTF::Shader* shader : shaders) {
			if (options->version == "1.0") {
				jsonWriter->Key(shader->getStringId().c_str());
			}
			jsonWriter->StartObject();
			shader->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	shaders.clear();

	// Write bufferViews and add buffers to the buffer array
	std::vector<std::shared_ptr<GLTF::Buffer>> buffers;
	if (bufferViews.size() > 0) {
		jsonWriter->Key("bufferViews");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (std::shared_ptr<GLTF::BufferView> bufferView : bufferViews) {
			if (bufferView->buffer) {
				std::shared_ptr<GLTF::Buffer> buffer = bufferView->buffer;
				if (buffer->id < 0) {
					buffer->id = buffers.size();
					buffers.push_back(buffer);
				}
			}
			if (options->version == "1.0") {
				jsonWriter->Key(bufferView->getStringId().c_str());
			}
			jsonWriter->StartObject();
			bufferView->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	bufferViews.clear();

	// Write buffers
	if (buffers.size() > 0) {
		jsonWriter->Key("buffers");
		if (options->version == "1.0") {
			jsonWriter->StartObject();
		}
		else {
			jsonWriter->StartArray();
		}
		for (auto const buffer : buffers) {
			if (options->version == "1.0") {
				jsonWriter->Key(buffer->getStringId().c_str());
			}
			jsonWriter->StartObject();
			buffer->writeJSON(writer, options);
			jsonWriter->EndObject();
		}
		if (options->version == "1.0") {
			jsonWriter->EndObject();
		}
		else {
			jsonWriter->EndArray();
		}
	}
	buffers.clear();

	// Write extensionsUsed and extensionsRequired
	if (this->extensionsRequired.size() > 0 && options->version != "1.0") {
		jsonWriter->Key("extensionsRequired");
		jsonWriter->StartArray();
		for (const std::string extension : this->extensionsRequired) {
			jsonWriter->String(extension.c_str());
		}
		jsonWriter->EndArray();
	}
	if (this->extensionsUsed.size() > 0) {
		jsonWriter->Key("extensionsUsed");
		jsonWriter->StartArray();
		for (const std::string extension : this->extensionsUsed) {
			jsonWriter->String(extension.c_str());
		}
		jsonWriter->EndArray();
	}

	GLTF::Object::writeJSON(writer, options);
}



//---------------------------------------------------------------------------------

/**Создать новый экземпляр */
std::shared_ptr<dgn::gltf::GltfAsset> dgn::gltf::GltfAsset::create(){
	return std::shared_ptr<dgn::gltf::GltfAsset>(
					new GLTF::Asset()
	);
}

/**Список корневых объектов сцены */
std::vector<std::shared_ptr<dgn::gltf::GltfNode>> GLTF::Asset::getNodes(){
	std::vector<std::shared_ptr<GLTF::Node>> &nd = this->getDefaultScene()->nodes;
	return std::vector<std::shared_ptr<dgn::gltf::GltfNode>>(nd.begin(), nd.end());
}

void GLTF::Asset::setNodes(const std::vector<std::shared_ptr<dgn::gltf::GltfNode>> & nd){
	std::vector<std::shared_ptr<GLTF::Node>> &nodes = this->getDefaultScene()->nodes;
	nodes.clear();
	for(std::shared_ptr<dgn::gltf::GltfNode> n : nd)
		nodes.push_back(std::dynamic_pointer_cast<GLTF::Node>(n));
}