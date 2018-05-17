/**
 * Утилиты на уровне подмодуля GLTF
 * @author Юрин Роман
 * @date 14.05.2018
 */
#pragma once

#include "dgn/gltf/GltfUtils.hpp"
#include <GLTFAsset.h>
#include <dgn/gltf/WebglConstants.hpp>
#include "dgn/gltf/Color.hpp"

namespace GLTF {
		namespace Utils {

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

				/**Техника отрисовки в Java**/
				dgn::gltf::MaterialTehnique	 tehniqueGltf2Java(GLTF::MaterialCommon::Technique gltf);

				/**Техника отрисовки из Java**/
				GLTF::MaterialCommon::Technique	 tehniqueJava2Gltf(dgn::gltf::MaterialTehnique gltf);

				/**Цвет из массива 4-х элементов**/
				std::optional<dgn::gltf::Color> colorArray2Java(float *vec4);

				/**Преобразовать режим построения примитива в Java формат**/
				dgn::gltf::GlMode primitiveModeGltf2Java(GLTF::Primitive::Mode mode);
				GLTF::Primitive::Mode primitiveModeJava2Gltf(dgn::gltf::GlMode mode);

				/**Цвет в массив из 4-х элементов**/
				float *colorJava2Array(const std::optional<dgn::gltf::Color> clr);
		};
}