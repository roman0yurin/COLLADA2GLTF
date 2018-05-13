#pragma once

#include <vector>

#include "GLTFObject.h"
#include "GLTFPrimitive.h"
#include "dgn/gltf/GltfMesh.hpp"

namespace GLTF {
	class Mesh : public GLTF::Object, public dgn::gltf::GltfMesh {
	public:
		std::vector<std::shared_ptr<GLTF::Primitive>> primitives;
		std::vector<float> weights;

		virtual std::string typeName();
		virtual std::shared_ptr<GLTF::Object> clone(std::shared_ptr<GLTF::Object> clone);
		virtual void writeJSON(void* writer, GLTF::Options* options);

		//--------------------------------------------------------------------------------------

		/**Набор графических примитивов, составляющих данный объект */
		std::vector<std::shared_ptr<dgn::gltf::GltfPrimitive>> getPrimitives();

		void setPrimitives(const std::vector<std::shared_ptr<dgn::gltf::GltfPrimitive>> & p);
	};
}
