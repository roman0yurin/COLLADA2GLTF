#pragma once

#include "GLTFBuffer.h"
#include "GLTFConstants.h"
#include "GLTFObject.h"
#include "dgn/gltf/GltfBufferView.hpp"
#include <dgn/gltf/WebglConstants.hpp>

namespace GLTF {
	class BufferView : public GLTF::Object, public dgn::gltf::GltfBufferView {
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


		//---------------------------------------------------------------------------------

		/**Сдвиг от начала буфера к началу области */
		int32_t getByteOffset();

		void setByteOffset(int32_t offset);

		/**Сдвиг между компонентами. 0 - значения идут подряд */
		int32_t getByteStride();

		void setByteStride(int32_t stride);

		/**Размер области в байтах */
		int32_t getByteLength();

		void setByteLength(int32_t len);

		/**Буфер из которого читаются данные */
		std::shared_ptr<dgn::gltf::GltfBuffer> getBuffer();

		void setBuffer(const std::shared_ptr<dgn::gltf::GltfBuffer> & b);

		/**Какая структура WebGL описывается в этой области данных */
		dgn::gltf::WebglConstants getTarget();

		void setTarget(dgn::gltf::WebglConstants webConst);
	};
};
