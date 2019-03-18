#pragma once

#include "GLTFOptions.h"

#include <string>
#include "dgn/gltf/GltfOptions.hpp"

namespace COLLADA2GLTF {
	/**Расширение для сжатия геометрии**/
	static const char* DRACO_EXTENSION = "KHR_draco_mesh_compression";

	class Options : public GLTF::Options, public dgn::gltf::GltfOptions{
	public:
		std::string inputPath;
		std::string basePath;
		std::string outputPath;

		/**Путь к файлу COLLADA для преобразования */
		std::string getInputPath() override;

		void setInputPath(const std::string & value) override;

		/**Папка относительно которой задаются пути */
		std::string getBasePath() override;

		void setBasePath(const std::string & value) override;

		/**Путь для вывода glTF файла */
		std::string getOutputPath() override;

		void setOutputPath(const std::string & value) override;

		/**описание вершинных буферов внутри основного файла */
		bool getEmbeddedBuffers() override;

		void setEmbeddedBuffers(bool value) override;

		/**описывать текстуры в основном файле */
		bool getEmbeddedTexture() override;

		void setEmbeddedTexture(bool value) override;

		/**шейдеры для материалов в основном файле */
		bool getEmbeddedShaders() override;

		void setEmbeddedShaders(bool value) override;

		/**вывод glTF в бинарном или json формате */
		bool getBinary() override;

		void setBinary(bool value) override;

		/**Описание материалов через общий формат */
		bool getMaterialsCommon() override;

		void setMaterialsCommon(bool value) override;

		/**Описание материалов через шейдеры (не совместим с getMaterialsCommon) */
		bool getGlSl() override;

		void setGlSl(bool value) override;

		/**Отображать изнаночную сторону поверхности */
		bool getDoubleSided() override;

		void setDoubleSided(bool value) override;

		/**Зеркальное отражение */
		bool getSpecularGlossiness() override;

		void setSpecularGlossiness(bool value) override;

		/**версия glTF формата для вывода, может быть 1.0 или 2.0 */
		std::string getVersion() override;

		void setVersion(const std::string & value) override;

		/**текстуры описывающие шероховатость металлической поверхности */
		std::vector<std::string> getMetallicRoughnessTexturePaths() override;

		void setMetallicRoughnessTexturePaths(const std::vector<std::string> & value) override;

		/**Использовать алгоритм сжатия "Дракон" */
		bool getDracoCompression() override;

		void setDracoCompression(bool value) override;

		/**Дискретизация положения вершин */
		int32_t getPositionQuantizationBits() override;

		void setPositionQuantizationBits(int32_t value) override;

		/**Дискретизация записи нормалей */
		int32_t getNormalQuantizationBits() override;

		void setNormalQuantizationBits(int32_t value) override;

		/**Дискретизация записи текстурных координат */
		int32_t getTexcoordQuantizationBits() override;

		void setTexcoordQuantizationBits(int32_t value) override;

		/**Дискретизация записи цветов */
		int32_t getColorQuantizationBits() override;

		void setColorQuantizationBits(int32_t value) override;

		int32_t getJointQuantizationBits() override;

		void setJointQuantizationBits(int32_t value) override;
	};
}