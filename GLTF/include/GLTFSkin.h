#pragma once

#include <vector>

#include "GLTFAccessor.h"
#include "GLTFObject.h"

namespace GLTF {
	class Node;

	class Skin : public GLTF::Object {
	public:
		std::shared_ptr<Accessor> inverseBindMatrices;
		std::weak_ptr<Node> skeleton;
		std::vector<std::weak_ptr<Node>> joints;

		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
