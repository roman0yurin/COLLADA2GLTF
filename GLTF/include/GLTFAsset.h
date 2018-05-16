#pragma once

#include <set>
#include <string>
#include <vector>

#include "GLTFAnimation.h"
#include "GLTFDracoExtension.h"
#include "GLTFObject.h"
#include "GLTFScene.h"

#include "draco/compression/encode.h"
#include "dgn/gltf/GltfAsset.hpp"

namespace GLTF {
	class Asset : public GLTF::Object, public dgn::gltf::GltfAsset {
	private:
		std::vector<std::shared_ptr<GLTF::MaterialCommon::Light>> _ambientLights;
	public:
		class Metadata : public GLTF::Object {
		public:
			std::string copyright;
			std::string generator = "COLLADA2GLTF";
			std::string version = "2.0";
			virtual void writeJSON(void* writer, GLTF::Options* options);
		};

		std::shared_ptr<GLTF::Sampler> globalSampler;

		std::shared_ptr<Metadata> metadata;
		std::set<std::string> extensionsUsed;
		std::set<std::string> extensionsRequired;

		std::vector<std::shared_ptr<GLTF::Scene>> scenes;
		std::vector<std::shared_ptr<GLTF::Animation>> animations;
		int scene = -1;
		/**Предотвращает повторную загрузку изображений в пределах набора**/
		std::map<std::string, std::shared_ptr<GLTF::Image>> imageCache;

		Asset();
		std::shared_ptr<GLTF::Scene> getDefaultScene();
		std::vector<std::shared_ptr<GLTF::Accessor>> getAllAccessors();
		std::vector<std::shared_ptr<GLTF::Node>> getAllNodes();
		std::vector<std::shared_ptr<GLTF::Mesh>> getAllMeshes();
		std::vector<std::shared_ptr<GLTF::Primitive>> getAllPrimitives();
		std::vector<std::shared_ptr<GLTF::Skin>> getAllSkins();
		std::vector<std::shared_ptr<GLTF::Material>> getAllMaterials();
		std::vector<std::shared_ptr<GLTF::Technique>> getAllTechniques();
		std::vector<GLTF::Program*> getAllPrograms();
		std::vector<GLTF::Shader*> getAllShaders();
		std::vector<std::shared_ptr<GLTF::Texture>> getAllTextures();
		std::vector<std::shared_ptr<GLTF::Image>> getAllImages();
		std::vector<std::shared_ptr<GLTF::Accessor>> getAllPrimitiveAccessors(std::shared_ptr<GLTF::Primitive> primitive) const;
		void mergeAnimations();
		void removeUnusedSemantics();
		void removeUnusedNodes(GLTF::Options* options);
		std::shared_ptr<GLTF::Buffer> packAccessors();

		// Functions for Draco compression extension.
		std::vector<std::shared_ptr<GLTF::BufferView>> getAllCompressedBufferView();
		bool compressPrimitives(GLTF::Options* options);
		void removeUncompressedBufferViews();
		void removeAttributeFromDracoExtension(std::shared_ptr<GLTF::Primitive> primitive, const std::string &semantic);

		void requireExtension(std::string extension);
		void useExtension(std::string extension);
		virtual void writeJSON(void* writer, GLTF::Options* options);

		//-------------------------------------------------------------------------
		/**Список корневых объектов сцены */
		std::vector<std::shared_ptr<dgn::gltf::GltfNode>> getNodes();

		void setNodes(const std::vector<std::shared_ptr<dgn::gltf::GltfNode>> & nd);
	};
}
