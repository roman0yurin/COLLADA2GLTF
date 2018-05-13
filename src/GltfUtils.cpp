

#include <gltf/asset/GltfAsset.h>
#include "gltf/GltfOptions.h"
#include "GLTF_Utils.h"
#include "dgn/gltf/GltfUtils.hpp"

/**записать набор 3д данных в glTF согласно опциям */
void djinni_gen::GltfUtils::writeAssetToGlTF(const std::shared_ptr<djinni_gen::GltfAsset> & asset, const std::shared_ptr<djinni_gen::GltfOptions> & options){
	GLTF::Utils::writeAssetToGlTF(dynamic_cast<GLTF::Asset*>(asset.get()), dynamic_cast<COLLADA2GLTF::Options*>(options.get()));
}

/*Цвет из четырех-компонентного вектора*/
std::optional<dgn::gltf::Color> GLTF::Utils::getColor(float *arr){
	if(arr){
		return std::optional(
				dgn::gltf::Color(arr[0], arr[1], arr[2], arr[3])
		);
	}else{
		return std::nullopt;
	}
}

/**Цвет в вектор**/
float * GLTF::Utils::fromColor(std::optional<dgn::gltf::Color> color){
	if(color.has_value())
		return new float[4]{color->red, color->green, color->blue, color->alpha};
	else
		return NULL;
}

/** Переход к перечислению GLTF **/
dgn::gltf::GltfDataStruct GLTF::Utils::dataStructGLTF2Java(Type gltf){
	switch(gltf){
		case Type::SCALAR:
			return dgn::gltf::GltfDataStruct::SCALAR;
		case Type::VEC2:
			return dgn::gltf::GltfDataStruct::VEC2;
		case Type::VEC3:
			return dgn::gltf::GltfDataStruct::VEC3;
		case Type::VEC4:
			return dgn::gltf::GltfDataStruct::VEC4;
		case Type::MAT2:
			return dgn::gltf::GltfDataStruct::MAT2;
		case Type::MAT3:
			return dgn::gltf::GltfDataStruct::MAT3;
		case Type::MAT4:
			return dgn::gltf::GltfDataStruct::MAT4;
		case Type::UNKNOWN:
			return dgn::gltf::GltfDataStruct::UNKNOWN;
		default:
			throw std::runtime_error("unexpected case");
	}
}

/**Переход к Java модели перечисления**/
Accessor::Type GLTF::Utils::dataStructJava2GLTF(dgn::gltf::GltfDataStruct javaStruct){
	switch (s){
		case dgn::gltf::GltfDataStruct::SCALAR:
			return this->type = Type ::SCALAR;
		case dgn::gltf::GltfDataStruct::VEC2:
			return this->type = Type ::VEC2;
		case dgn::gltf::GltfDataStruct::VEC3:
			return this->type = Type ::VEC3
		case dgn::gltf::GltfDataStruct::VEC4:
			return this->type = Type ::VEC4;
		case dgn::gltf::GltfDataStruct::MAT2:
			return this->type = Type ::MAT2;
		case dgn::gltf::GltfDataStruct::MAT3:
			return this->type = Type ::MAT3;
		case dgn::gltf::GltfDataStruct::MAT4:
			return this->type = Type ::MAT4;
		case dgn::gltf::GltfDataStruct::UNKNOWN:
			return this->type = Type ::UNKNOWN;
		default:
			throw std::runtime_error("Unexpected case");
	}
}


/**Преобразовать константу к java варианту**/
dgn::gltf::WebglConstants GLTF::Utils::webglConstGLTF2Java(GLTF::Constants::WebGL gltf){
	switch(gltf){
		case GLTF::Constants::WebGL::ALPHA:
			return dgn::gltf::WebglConstants::ALPHA;
		case GLTF::Constants::WebGL::ARRAY_BUFFER:
			return dgn::gltf::WebglConstants::ARRAY_BUFFER;
		case GLTF::Constants::WebGL::BOOL:
			return dgn::gltf::WebglConstants::BOOL;
		case GLTF::Constants::WebGL::BOOL_VEC2:
			return dgn::gltf::WebglConstants::BOOL_VEC2;
		case GLTF::Constants::WebGL::BOOL_VEC3:
			return dgn::gltf::WebglConstants::BOOL_VEC3;
		case GLTF::Constants::WebGL::BOOL_VEC4:
			return dgn::gltf::WebglConstants::BOOL_VEC4;
		case GLTF::Constants::WebGL::BYTE:
			return dgn::gltf::WebglConstants::BYTE;
		case GLTF::Constants::WebGL::CLAMP_TO_EDGE:
			return dgn::gltf::WebglConstants::CLAMP_TO_EDGE;
		case GLTF::Constants::WebGL::CULL_FACE:
			return dgn::gltf::WebglConstants::CULL_FACE;
		case GLTF::Constants::WebGL::DEPTH_TEST:
			return dgn::gltf::WebglConstants::DEPTH_TEST;
		case GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER:
			return dgn::gltf::WebglConstants::ELEMENT_ARRAY_BUFFER;
		case GLTF::Constants::WebGL::FLOAT:
			return dgn::gltf::WebglConstants::FLOAT;
		case GLTF::Constants::WebGL::FLOAT_MAT2:
			return dgn::gltf::WebglConstants::FLOAT_MAT2;
		case GLTF::Constants::WebGL::FLOAT_MAT3:
			return dgn::gltf::WebglConstants::FLOAT_MAT3;
		case GLTF::Constants::WebGL::FLOAT_MAT4:
			return dgn::gltf::WebglConstants::FLOAT_MAT4;
		case GLTF::Constants::WebGL::FLOAT_VEC2:
			return dgn::gltf::WebglConstants::FLOAT_VEC2;
		case GLTF::Constants::WebGL::FLOAT_VEC3:
			return dgn::gltf::WebglConstants::FLOAT_VEC3;
		case GLTF::Constants::WebGL::FLOAT_VEC4:
			return dgn::gltf::WebglConstants::FLOAT_VEC4;
		case GLTF::Constants::WebGL::FRAGMENT_SHADER:
			return dgn::gltf::WebglConstants::FRAGMENT_SHADER;
		case GLTF::Constants::WebGL::FUNC_ADD:
			return dgn::gltf::WebglConstants::FUNC_ADD;
		case GLTF::Constants::WebGL::INT_VEC2:
			return dgn::gltf::WebglConstants::INT_VEC2;
		case GLTF::Constants::WebGL::INT_VEC3:
			return dgn::gltf::WebglConstants::INT_VEC3;
		case GLTF::Constants::WebGL::INT_VEC4:
			return dgn::gltf::WebglConstants::INT_VEC4;
		case GLTF::Constants::WebGL::LINEAR:
			return dgn::gltf::WebglConstants::LINEAR;
		case GLTF::Constants::WebGL::LINEAR_MIPMAP_LINEAR:
			return dgn::gltf::WebglConstants::LINEAR_MIPMAP_LINEAR;
		case GLTF::Constants::WebGL::LINEAR_MIPMAP_NEAREST:
			return dgn::gltf::WebglConstants::LINEAR_MIPMAP_NEAREST;
		case GLTF::Constants::WebGL::LUMINANCE:
			return dgn::gltf::WebglConstants::LUMINANCE;
		case GLTF::Constants::WebGL::LUMINANCE_ALPHA:
			return dgn::gltf::WebglConstants::LUMINANCE_ALPHA;
		case GLTF::Constants::WebGL::MIRRORED_REPEAT:
			return dgn::gltf::WebglConstants::MIRRORED_REPEAT;
		case GLTF::Constants::WebGL::NEAREST:
			return dgn::gltf::WebglConstants::NEAREST;
		case GLTF::Constants::WebGL::NEAREST_MIPMAP_NEAREST:
			return dgn::gltf::WebglConstants::NEAREST_MIPMAP_NEAREST;
		case GLTF::Constants::WebGL::NEAREST_MIPMAP_LINEAR:
			return dgn::gltf::WebglConstants::NEAREST_MIPMAP_LINEAR;
		case GLTF::Constants::WebGL::ONE:
			return dgn::gltf::WebglConstants::ONE;
		case GLTF::Constants::WebGL::ONE_MINUS_SRC_ALPHA:
			return dgn::gltf::WebglConstants::ONE_MINUS_SRC_ALPHA;
		case GLTF::Constants::WebGL::REPEAT:
			return dgn::gltf::WebglConstants::REPEAT;
		case GLTF::Constants::WebGL::RGBA:
			return dgn::gltf::WebglConstants::RGBA;
		case GLTF::Constants::WebGL::SAMPLER_2D:
			return dgn::gltf::WebglConstants::SAMPLER_2D;
		case GLTF::Constants::WebGL::SAMPLER_CUBE:
			return dgn::gltf::WebglConstants::SAMPLER_CUBE;
		case GLTF::Constants::WebGL::SHORT:
			return dgn::gltf::WebglConstants::SHORT;
		case GLTF::Constants::WebGL::TEXTURE_2D:
			return dgn::gltf::WebglConstants::TEXTURE_2D;
		case GLTF::Constants::WebGL::UNSIGNED_BYTE:
			return dgn::gltf::WebglConstants::UNSIGNED_BYTE;
		case GLTF::Constants::WebGL::UNSIGNED_INT:
			return dgn::gltf::WebglConstants::UNSIGNED_INT;
		case GLTF::Constants::WebGL::UNSIGNED_SHORT:
			return dgn::gltf::WebglConstants::UNSIGNED_SHORT;
		case GLTF::Constants::WebGL::VERTEX_SHADER:
			return dgn::gltf::WebglConstants::VERTEX_SHADER;
		default:
			throw std::runtime_error("unexpected case");
	}
}


/**Преобразовать константу к java варианту**/
dgn::gltf::WebglConstants webglJava2GLTF(dgn::gltf::WebglConstants javaConst){
	switch(javaConst){
		case dgn::gltf::WebglConstants::ALPHA:
			return GLTF::Constants::WebGL::ALPHA;
		case dgn::gltf::WebglConstants::ARRAY_BUFFER:
			return GLTF::Constants::WebGL::ARRAY_BUFFER;
		case dgn::gltf::WebglConstants::BOOL:
			return GLTF::Constants::WebGL::BOOL;
		case dgn::gltf::WebglConstants::BOOL_VEC2:
			return GLTF::Constants::WebGL::BOOL_VEC2;
		case dgn::gltf::WebglConstants::BOOL_VEC3:
			return GLTF::Constants::WebGL::BOOL_VEC3;
		case dgn::gltf::WebglConstants::BOOL_VEC4:
			return GLTF::Constants::WebGL::BOOL_VEC4;
		case dgn::gltf::WebglConstants::BYTE:
			return GLTF::Constants::WebGL::BYTE;
		case dgn::gltf::WebglConstants::CLAMP_TO_EDGE:
			return GLTF::Constants::WebGL::CLAMP_TO_EDGE;
		case dgn::gltf::WebglConstants::CULL_FACE:
			return GLTF::Constants::WebGL::CULL_FACE;
		case dgn::gltf::WebglConstants::DEPTH_TEST:
			return GLTF::Constants::WebGL::DEPTH_TEST;
		case dgn::gltf::WebglConstants::ELEMENT_ARRAY_BUFFER:
			return GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER;
		case dgn::gltf::WebglConstants::FLOAT:
			return GLTF::Constants::WebGL::FLOAT;
		case dgn::gltf::WebglConstants::FLOAT_MAT2:
			return GLTF::Constants::WebGL::FLOAT_MAT2;
		case dgn::gltf::WebglConstants::FLOAT_MAT3:
			return GLTF::Constants::WebGL::FLOAT_MAT3;
		case dgn::gltf::WebglConstants::FLOAT_MAT4:
			return GLTF::Constants::WebGL::FLOAT_MAT4;
		case dgn::gltf::WebglConstants::FLOAT_VEC2:
			return GLTF::Constants::WebGL::FLOAT_VEC2;
		case dgn::gltf::WebglConstants::FLOAT_VEC3:
			return GLTF::Constants::WebGL::FLOAT_VEC3;
		case dgn::gltf::WebglConstants::FLOAT_VEC4:
			return GLTF::Constants::WebGL::FLOAT_VEC4;
		case dgn::gltf::WebglConstants::FRAGMENT_SHADER:
			return GLTF::Constants::WebGL::FRAGMENT_SHADER;
		case dgn::gltf::WebglConstants::FUNC_ADD:
			return GLTF::Constants::WebGL::FUNC_ADD;
		case dgn::gltf::WebglConstants::INT_VEC2:
			return GLTF::Constants::WebGL::INT_VEC2;
		case dgn::gltf::WebglConstants::INT_VEC3:
			return GLTF::Constants::WebGL::INT_VEC3;
		case dgn::gltf::WebglConstants::INT_VEC4:
			return GLTF::Constants::WebGL::INT_VEC4;
		case dgn::gltf::WebglConstants::LINEAR:
			return GLTF::Constants::WebGL::LINEAR;
		case dgn::gltf::WebglConstants::LINEAR_MIPMAP_LINEAR:
			return GLTF::Constants::WebGL::LINEAR_MIPMAP_LINEAR;
		case dgn::gltf::WebglConstants::LINEAR_MIPMAP_NEAREST:
			return GLTF::Constants::WebGL::LINEAR_MIPMAP_NEAREST;
		case dgn::gltf::WebglConstants::LUMINANCE:
			return GLTF::Constants::WebGL::LUMINANCE;
		case dgn::gltf::WebglConstants::LUMINANCE_ALPHA:
			return GLTF::Constants::WebGL::LUMINANCE_ALPHA;
		case dgn::gltf::WebglConstants::MIRRORED_REPEAT:
			return GLTF::Constants::WebGL::MIRRORED_REPEAT;
		case dgn::gltf::WebglConstants::NEAREST:
			return GLTF::Constants::WebGL::NEAREST;
		case dgn::gltf::WebglConstants::NEAREST_MIPMAP_NEAREST:
			return GLTF::Constants::WebGL::NEAREST_MIPMAP_NEAREST;
		case dgn::gltf::WebglConstants::NEAREST_MIPMAP_LINEAR:
			return GLTF::Constants::WebGL::NEAREST_MIPMAP_LINEAR;
		case dgn::gltf::WebglConstants::ONE:
			return GLTF::Constants::WebGL::ONE;
		case dgn::gltf::WebglConstants::ONE_MINUS_SRC_ALPHA:
			return GLTF::Constants::WebGL::ONE_MINUS_SRC_ALPHA;
		case dgn::gltf::WebglConstants::REPEAT:
			return GLTF::Constants::WebGL::REPEAT;
		case dgn::gltf::WebglConstants::RGBA:
			return GLTF::Constants::WebGL::RGBA;
		case dgn::gltf::WebglConstants::SAMPLER_2D:
			return GLTF::Constants::WebGL::SAMPLER_2D;
		case dgn::gltf::WebglConstants::SAMPLER_CUBE:
			return GLTF::Constants::WebGL::SAMPLER_CUBE;
		case dgn::gltf::WebglConstants::SHORT:
			return GLTF::Constants::WebGL::SHORT;
		case dgn::gltf::WebglConstants::TEXTURE_2D:
			return GLTF::Constants::WebGL::TEXTURE_2D;
		case dgn::gltf::WebglConstants::UNSIGNED_BYTE:
			return GLTF::Constants::WebGL::UNSIGNED_BYTE;
		case dgn::gltf::WebglConstants::UNSIGNED_INT:
			return GLTF::Constants::WebGL::UNSIGNED_INT;
		case dgn::gltf::WebglConstants::UNSIGNED_SHORT:
			return GLTF::Constants::WebGL::UNSIGNED_SHORT;
		case dgn::gltf::WebglConstants::VERTEX_SHADER:
			return GLTF::Constants::WebGL::VERTEX_SHADER;
		default:
			throw std::runtime_error("unexpected case");
	}
}


/**Тип аттрибута в формате Java**/
dgn::gltf::GltfAttributeType GLTF::Utils::attrTypeGltf2Java(std::string gltf){
	switch(gltf){
		case "POSITION":
			return dgn::gltf::GltfAttributeType::POSITION;
		case "NORMAL":
			return dgn::gltf::GltfAttributeType::NORMAL;
		case "TANGENT":
			return dgn::gltf::GltfAttributeType::TANGENT;
		case "TEXCOORD_0":
			return dgn::gltf::GltfAttributeType::TEXCOORD0;
		case "TEXCOORD_1":
			return dgn::gltf::GltfAttributeType::TXTCOORD1;
		case "COLOR_0":
			return dgn::gltf::GltfAttributeType::COLOR0;
		case "JOINS_0":
			return dgn::gltf::GltfAttributeType::JOINS0;
		case "WEIGHTS_0":
			return dgn::gltf::GltfAttributeType::WEIGHTS0;
		default:
			throw std::runtime_error("Unexpected case");

	}
}

/**Тип аттрибута в формате GLTF**/
std::string GLTF::Utils::attrTypeJava2Gltf(dgn::gltf::GltfAttributeType java){
	switch(java){
		case  dgn::gltf::GltfAttributeType::POSITION:
			return "POSITION";
		case dgn::gltf::GltfAttributeType::NORMAL:
			return "NORMAL";
		case dgn::gltf::GltfAttributeType::TANGENT:
			return "TANGENT";
		case dgn::gltf::GltfAttributeType::TEXCOORD0:
			return "TEXCOORD_0";
		case dgn::gltf::GltfAttributeType::TXTCOORD1:
			return "TEXCOORD_1";
		case dgn::gltf::GltfAttributeType::COLOR0:
			return "COLOR_0";
		case dgn::gltf::GltfAttributeType::JOINS0:
			return "JOINS_0";
		case dgn::gltf::GltfAttributeType::WEIGHTS0:
			return "WEIGHTS_0";
		default:
			throw std::runtime_error("Unexpected case");
	}
}



