#pragma once

#include <string>
#include <cmath>

#include "GLTFObject.h"
#include "GLTFTechnique.h"
#include "GLTFTexture.h"
#include "dgn/gltf/MaterialCommon.hpp"
#include "dgn/gltf/MaterialTehnique.hpp"

namespace GLTF {
	class Material : public GLTF::Object {
	public:
		enum Type {
			MATERIAL,
			MATERIAL_COMMON,
			PBR_METALLIC_ROUGHNESS,
			UNKNOWN
		};

		class Values {
		public:
			std::unique_ptr<float[]> ambient;
			std::shared_ptr<GLTF::Texture> ambientTexture;
			std::unique_ptr<float[]> diffuse;
			std::shared_ptr<GLTF::Texture> diffuseTexture;
			std::unique_ptr<float[]> emission;
			std::shared_ptr<GLTF::Texture> emissionTexture;
			std::unique_ptr<float[]> specular;
			std::shared_ptr<GLTF::Texture> specularTexture;
			std::unique_ptr<float[]> shininess;
			std::unique_ptr<float> transparency;
			std::shared_ptr<GLTF::Texture> bumpTexture;

			void writeJSON(void* writer, GLTF::Options* options);
		};

		std::shared_ptr<GLTF::Technique> technique;
		Type type = Type::UNKNOWN;
		std::shared_ptr<Values> values = NULL;
		bool doubleSided = false;

		Material();
		bool hasTexture();
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};

	class MaterialPBR : public GLTF::Material {
	public: 
		class Texture : public GLTF::Object {
		public:
			float scale = 1;
			std::shared_ptr<GLTF::Texture> texture;
			int texCoord = -1;

			void writeJSON(void* writer, GLTF::Options* options);
		};

		class MetallicRoughness : public GLTF::Object {
		public:
			std::unique_ptr<float[]> baseColorFactor;
			std::shared_ptr<Texture> baseColorTexture;
			float metallicFactor = -1.0;
			float roughnessFactor = -1.0;
			std::shared_ptr<Texture> metallicRoughnessTexture;

			void writeJSON(void* writer, GLTF::Options* options);
		};

		class SpecularGlossiness : public GLTF::Object {
		public:
			std::unique_ptr<float[]> diffuseFactor;
			std::shared_ptr<Texture> diffuseTexture;
			std::unique_ptr<float[]> specularFactor;
			std::shared_ptr<Texture> specularGlossinessTexture;
			std::unique_ptr<float> glossinessFactor;

			void writeJSON(void* writer, GLTF::Options* options);
		};

		std::unique_ptr<MetallicRoughness> metallicRoughness;
		std::shared_ptr<Texture> normalTexture;
		std::shared_ptr<Texture> occlusionTexture;
		std::unique_ptr<float[]> emissiveFactor;
		std::shared_ptr<Texture> emissiveTexture;
		std::shared_ptr<SpecularGlossiness> specularGlossiness;

		/** Either "OPAQUE", "BLEND" or "MASK". Default = "OPAQUE" */
		std::string alphaMode;

		/** Only when alphaMode == "MASK". Default = 0.5 */
		float alphaCutoff = NAN;

		bool doubleSided = false;

		MaterialPBR();
		void writeJSON(void* writer, GLTF::Options* options);
	};

	class MaterialCommon : public GLTF::Material, public dgn::gltf::MaterialCommon {
	public:
		enum Technique {
			BLINN,
			PHONG,
			LAMBERT,
			CONSTANT,
			UNKNOWN
		};

		class Light: public GLTF::Object {
		public:
			enum Type {
				AMBIENT,
				DIRECTIONAL,
				POINT,
				SPOT,
				UNKOWN
			};

			Type type = Type::UNKOWN;
			float color[4];
			float constantAttenuation;
			float linearAttenuation;
			float quadraticAttenuation;
			std::weak_ptr<GLTF::Object> node;

			virtual std::string typeName();
			virtual void writeJSON(void* writer, GLTF::Options* options);
		};

		int jointCount = 0;
		bool transparent = false;

		MaterialCommon::Technique technique = MaterialCommon::Technique::UNKNOWN;

		MaterialCommon();
		const char* getTechniqueName();
		std::shared_ptr<GLTF::Material> getMaterial(std::vector<std::shared_ptr<GLTF::MaterialCommon::Light>> lights, GLTF::Options* options);
		std::shared_ptr<GLTF::Material> getMaterial(std::vector<std::shared_ptr<GLTF::MaterialCommon::Light>> lights, bool hasColorAttribute, GLTF::Options* options);
		std::string getTechniqueKey(GLTF::Options* options);
		std::shared_ptr<GLTF::MaterialPBR> getMaterialPBR(GLTF::Options* options);
		virtual void writeJSON(void* writer, GLTF::Options* options);


		//------------------------------------------------------------------------------------------------------------------

			/**Материал явялется прозрачным */
			bool isTransparent();

			void setTransparent(bool value);

			/**Отображать материал с двух сторон */
			bool isDoubleSided();

			void setDoubleSided(bool value) ;

			/**Способ отображения материала */
			dgn::gltf::MaterialTehnique getTehnique();

			void setTehnique(dgn::gltf::MaterialTehnique value) ;

			/**Освещение от окружающих предметов */
			std::optional<dgn::gltf::Color> getAmbient();

			void setAmbient(const std::optional<dgn::gltf::Color> & value);

			/**Освещение от рассеивания света поверхностью предмета */
			std::optional<dgn::gltf::Color> getDiffuse();

			void setDiffuse(const std::optional<dgn::gltf::Color> & value);

			/**Собственный источник свет */
			std::optional<dgn::gltf::Color> getEmission();

			void setEmission(const std::optional<dgn::gltf::Color> & value);

			/**отражение источника света */
			std::optional<dgn::gltf::Color> getSpecular();

			void setSpecular(const std::optional<dgn::gltf::Color> & value);

			/**Коэф. отражения */
			std::optional<float> getShininess();

			void setShininess(std::optional<float> value);

			/**Коэф. прозрачности */
			std::optional<float> getTransparency();

			void setTransparency(std::optional<float> value);
	};
}
