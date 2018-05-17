#pragma once

#include <map>
#include <string>
#include <dgn/gltf/GltfAttributeType.hpp>
#include <dgn/gltf/GlMode.hpp>

#include "GLTFAccessor.h"
#include "GLTFDracoExtension.h"
#include "GLTFMaterial.h"
#include "GLTFObject.h"
#include "dgn/gltf/GltfPrimitive.hpp"

namespace GLTF {
	class Primitive : public GLTF::Object, public dgn::gltf::GltfPrimitive {
	public:
		enum Mode {
			UNKNOWN = -1,
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6,
		};

		class Target {
		public:
			std::map<std::string, std::shared_ptr<GLTF::Accessor>> attributes;

			Target* clone(GLTF::Object* clone);
			void writeJSON(void* writer, GLTF::Options* options);
		};

		std::map<std::string, std::shared_ptr<GLTF::Accessor>> attributes;
		std::shared_ptr<GLTF::Accessor> indices = NULL;
		std::shared_ptr<GLTF::Material> material = NULL;
		Mode mode = Mode::UNKNOWN;
		std::vector<std::shared_ptr<Target>> targets;

		virtual std::shared_ptr<GLTF::Object> clone(std::shared_ptr<GLTF::Object> clone);
		virtual void writeJSON(void* writer, GLTF::Options* options);


		//-----------------------------------------------------------------------------------
		/**Доступ к данным для соответствующего атрибута */
		std::shared_ptr<dgn::gltf::GltfAccessor> getAttribute(dgn::gltf::GltfAttributeType tp);

		void setAttribute(dgn::gltf::GltfAttributeType tp, const std::shared_ptr<dgn::gltf::GltfAccessor> & accessor);

		/**Материал задает графические свойства поверхности */
		std::shared_ptr<dgn::gltf::MaterialCommon> getMaterial();

		void setMaterial(const std::shared_ptr<dgn::gltf::MaterialCommon> & m);

		/**задет список вершин для построения примитивов */
		std::shared_ptr<dgn::gltf::GltfAccessor> getIndexes();

		void setIndexes(const std::shared_ptr<dgn::gltf::GltfAccessor> & idx);


		/**Режим построения примитива из вершинного массива */
		dgn::gltf::GlMode getMode();
		void setMode(dgn::gltf::GlMode m);
	};
}
