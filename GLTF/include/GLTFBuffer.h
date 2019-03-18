#pragma once

#include "GLTFObject.h"
#include "dgn/gltf/GltfBuffer.hpp"

namespace GLTF {
	class Buffer : public GLTF::Object, public dgn::gltf::GltfBuffer {
	public:
		std::unique_ptr<std::vector<uint8_t>> data;
		int byteLength;
		std::string uri;

		/**Становится обладателем**/
		Buffer(std::vector<uint8_t> *data, int dataLength);
		
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);

		/**Данные размещенные непосредственно в файле (а не по ссылке) */
		std::optional<std::vector<uint8_t>> getData() override;

		std::unique_ptr<std::vector<uint8_t >> &getDataRef();

		void setData(const std::optional<std::vector<uint8_t>> & data) override;

		/**Сссылка на данные */
		std::optional<std::string> getUri() override;

		void setUri(const std::optional<std::string> & uri) override;

		/**Если данные берутся через ссылку, то нужно указать длину */
		int32_t getLength() override;

		void setLength(int32_t len) override;
	};
};
