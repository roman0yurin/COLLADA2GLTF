#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "draco/compression/encode.h"
#include "GLTFAccessor.h"
#include "GLTFBufferView.h"
#include "GLTFConstants.h"
#include "GLTFObject.h"
#include "GLTFExtension.h"

namespace GLTF {
	class DracoExtension : public GLTF::Extension {
	public:
		DracoExtension() : dracoMesh(new draco::Mesh()) {} 
		std::shared_ptr<GLTF::BufferView> bufferView;
		std::unordered_map<std::string, int> attributeToId;
		
		std::unique_ptr<draco::Mesh> dracoMesh;
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
