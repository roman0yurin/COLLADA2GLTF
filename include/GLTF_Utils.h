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
#include "GLTFUtils.h"

namespace GLTF {
		namespace Utils {
			/**
			 * Выгрузить набор 3D данных в glTF формат согласно опциям
			 **/
			void writeAssetToGlTF(std::shared_ptr<GLTF::Asset> asset, COLLADA2GLTF::Options* options, const std::shared_ptr<dgn::gltf::JavaOutputSream> & output);

			/**Обеспечивает обращение к map как к кешу элементов.**/
			template <typename KEY, typename VALUE>
			VALUE &cache(std::map<KEY, VALUE> &storage, const KEY &key, const std::function<VALUE (KEY)> &calculate){
				const auto &iterator = storage.find(key);
				if(iterator != storage.end())
					return iterator->second;
				else{
					return storage.emplace(key, calculate(key)).first->second;
				}
			}

			/**Обращение к хранилищу со значением по умолчанию**/
			template <typename KEY, typename VALUE>
			VALUE &withDefault(std::map<KEY, VALUE> &storage, const KEY &key, const std::function<VALUE ()> &dafaultValue){
				const auto &iterator = storage.find(key);
				if(iterator != storage.end())
					return iterator->second;
				else{
					return storage.emplace(key, dafaultValue()).first->second;
				}
			}
		};
}

