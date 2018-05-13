#pragma once

#include "GLTFBuffer.h"
#include "GLTFConstants.h"
#include "GLTFObject.h"

namespace GLTF {
	class BufferView : public GLTF::Object {
	public:
		std::shared_ptr<GLTF::Buffer> buffer;
		int byteOffset = 0;
		int byteStride = 0;
		int byteLength = 0;
		GLTF::Constants::WebGL target = (GLTF::Constants::WebGL)-1;

		BufferView(int byteOffset, int byteLength, std::shared_ptr<GLTF::Buffer> buffer);

		/**Становится собственником data**/
		BufferView(std::vector<uint8_t> *data, int dataLength);

		/**Становится собственником data**/
		BufferView(std::vector<uint8_t> *data,
			int dataLength,
			GLTF::Constants::WebGL target
		);

		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
};
