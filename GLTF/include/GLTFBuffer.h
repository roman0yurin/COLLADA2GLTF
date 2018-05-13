#pragma once

#include "GLTFObject.h"

namespace GLTF {
	static int dodo = 0;
	class Buffer : public GLTF::Object {
		int currDodo = dodo++;
	public:
		std::unique_ptr<std::vector<uint8_t>> data;
		int byteLength;
		std::string uri;

		/**Становится обладателем**/
		Buffer(std::vector<uint8_t> *data, int dataLength);
		
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
};
