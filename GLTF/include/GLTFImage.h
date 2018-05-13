#pragma once

#include "GLTFBufferView.h"
#include "GLTFObject.h"

namespace GLTF {
	class Image : public GLTF::Object {
	public:
		std::string uri;
		std::unique_ptr<std::vector<uint8_t>> data;
		size_t byteLength = 0;
		std::string mimeType;
		std::shared_ptr<GLTF::BufferView> bufferView;

		Image(std::string uri);

		/**Получает управление над вектором data**/
		Image(std::string uri, std::vector<uint8_t> *data, size_t byteLength, std::string fileExtension);

		static std::shared_ptr<GLTF::Image> load(path path, std::map<std::string, std::shared_ptr<GLTF::Image>> &imageCache);
		std::pair<int, int> getDimensions();
		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);

	private:
		const std::string cacheKey;

		Image(std::string uri, std::string cacheKey);

		/**Получает управление над вектором data**/
		Image(std::string uri, std::string cacheKey, std::vector<uint8_t> *data, size_t byteLength, std::string fileExtension);
	};
}