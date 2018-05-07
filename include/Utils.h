/**
 * Утилиты для конвертации данных в glTF формат
 * @author Юрин Роман @date 07.05.2018
 **/
#pragma once

#include "COLLADA2GLTFWriter.h"
#include "COLLADA2GLTFExtrasHandler.h"
#include "COLLADASaxFWLLoader.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "ahoy/ahoy.h"

namespace glTF {
		namespace Utils {
			/**
			 * Выгрузить набор 3D данных в glTF формат согласно опциям
			 **/
			void writeAssetToGlTF(GLTF::Asset* asset, COLLADA2GLTF::Options* options, path *outputPath);
		};
}

