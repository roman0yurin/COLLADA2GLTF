#pragma once

#include <vector>

#include "GLTFObject.h"
#include "GLTFPrimitive.h"

namespace GLTF {
	class Mesh : public GLTF::Object {
	public:
		std::vector<std::shared_ptr<GLTF::Primitive>> primitives;
		std::vector<float> weights;

		virtual std::string typeName();
		virtual std::shared_ptr<GLTF::Object> clone(std::shared_ptr<GLTF::Object> clone);
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
