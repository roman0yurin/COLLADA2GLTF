/**
 * Утилиты для конвертации данных в glTF формат
 * @author Юрин Роман @date 07.05.2018
 **/
#pragma once

#include <GLTFAsset.h>
#include <dgn/gltf/WebglConstants.hpp>
#include "dgn/gltf/Color.hpp"
#include "COLLADA2GLTFOptions.h"
#include "dgn/gltf/GltfAttributeType.hpp"

namespace GLTF {
		namespace Utils {
			/**
			 * Выгрузить набор 3D данных в glTF формат согласно опциям
			 **/
			void writeAssetToGlTF(std::shared_ptr<GLTF::Asset> asset, COLLADA2GLTF::Options* options);

			/*Цвет из четырех-компонентного вектора*/
			std::optional<dgn::gltf::Color> getColor(float *arr);

			/**Цвет в вектор**/
			float * fromColor(std::optional<dgn::gltf::Color> color);

			/** Переход к перечислению GLTF **/
			dgn::gltf::GltfDataStruct dataStructGLTF2Java(Accessor::Type gltf);

			/**Переход к Java модели перечисления**/
			Accessor::Type dataStructJava2GLTF(dgn::gltf::GltfDataStruct javaStruct);

			/**Преобразовать константу к java варианту**/
			dgn::gltf::WebglConstants webglConstGLTF2Java(GLTF::Constants::WebGL gltf);

			/**Преобразовать константу к java варианту**/
			GLTF::Constants::WebGL webglJava2GLTF(dgn::gltf::WebglConstants javaConst);

			/**Тип аттрибута в формате Java**/
			dgn::gltf::GltfAttributeType attrTypeGltf2Java(std::string gltf);

			/**Тип аттрибута в формате GLTF**/
			std::string attrTypeJava2Gltf(dgn::gltf::GltfAttributeType java);
		};
}

