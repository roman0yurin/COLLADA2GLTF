#include <dgn/gltf/GltfAttributeType.hpp>
#include "GLTFUtils.h"

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
dgn::gltf::GltfDataStruct GLTF::Utils::dataStructGLTF2Java(GLTF::Accessor::Type gltf){
	switch(gltf){
		case GLTF::Accessor::Type::SCALAR:
			return dgn::gltf::GltfDataStruct::SCALAR;
		case GLTF::Accessor::Type::VEC2:
			return dgn::gltf::GltfDataStruct::VEC2;
		case GLTF::Accessor::Type::VEC3:
			return dgn::gltf::GltfDataStruct::VEC3;
		case GLTF::Accessor::Type::VEC4:
			return dgn::gltf::GltfDataStruct::VEC4;
		case GLTF::Accessor::Type::MAT2:
			return dgn::gltf::GltfDataStruct::MAT2;
		case GLTF::Accessor::Type::MAT3:
			return dgn::gltf::GltfDataStruct::MAT3;
		case GLTF::Accessor::Type::MAT4:
			return dgn::gltf::GltfDataStruct::MAT4;
		case GLTF::Accessor::Type::UNKNOWN:
			return dgn::gltf::GltfDataStruct::UNKNOWN;
		default:
			throw std::runtime_error("unexpected case");
	}
}

/**Переход к Java модели перечисления**/
GLTF::Accessor::Type GLTF::Utils::dataStructJava2GLTF(dgn::gltf::GltfDataStruct javaStruct){
	switch (javaStruct){
		case dgn::gltf::GltfDataStruct::SCALAR:
			return GLTF::Accessor::Type::SCALAR;
		case dgn::gltf::GltfDataStruct::VEC2:
			return GLTF::Accessor::Type::VEC2;
		case dgn::gltf::GltfDataStruct::VEC3:
			return GLTF::Accessor::Type::VEC3;
		case dgn::gltf::GltfDataStruct::VEC4:
			return GLTF::Accessor::Type::VEC4;
		case dgn::gltf::GltfDataStruct::MAT2:
			return GLTF::Accessor::Type::MAT2;
		case dgn::gltf::GltfDataStruct::MAT3:
			return GLTF::Accessor::Type::MAT3;
		case dgn::gltf::GltfDataStruct::MAT4:
			return GLTF::Accessor::Type::MAT4;
		case dgn::gltf::GltfDataStruct::UNKNOWN:
			return GLTF::Accessor::Type::UNKNOWN;
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
GLTF::Constants::WebGL GLTF::Utils::webglJava2GLTF(dgn::gltf::WebglConstants javaConst){
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
	if(gltf.compare("POSITION"))
		return dgn::gltf::GltfAttributeType::POSITION;
	else if(gltf.compare("NORMAL"))
		return dgn::gltf::GltfAttributeType::NORMAL;
	else if(gltf.compare("TANGENT"))
		return dgn::gltf::GltfAttributeType::TANGENT;
	else if(gltf.compare("TEXCOORD_0"))
		return dgn::gltf::GltfAttributeType::TEXCOORD0;
	else if(gltf.compare("TEXCOORD_1"))
		return dgn::gltf::GltfAttributeType::TEXCOORD1;
	else if(gltf.compare("TEXCOORD_0"))
		return dgn::gltf::GltfAttributeType::TEXCOORD0;
	else if(gltf.compare("COLOR_0"))
		return dgn::gltf::GltfAttributeType::COLOR0;
	else if(gltf.compare("JOINS_0"))
		return dgn::gltf::GltfAttributeType::JOINS0;
	else if(gltf.compare("WEIGHTS_0"))
		return dgn::gltf::GltfAttributeType::WEIGHTS0;
	else
		throw std::runtime_error("Unexpected case");
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
		case dgn::gltf::GltfAttributeType::TEXCOORD1:
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


/**Техника отрисовки в Java**/
dgn::gltf::MaterialTehnique	 GLTF::Utils::tehniqueGltf2Java(GLTF::MaterialCommon::Technique gltf){
	switch (gltf){
		case GLTF::MaterialCommon::Technique::BLINN:
			return dgn::gltf::MaterialTehnique::BLINN;
		case GLTF::MaterialCommon::Technique::CONSTANT:
			return dgn::gltf::MaterialTehnique::CONSTANT;
		case GLTF::MaterialCommon::Technique::LAMBERT:
			return dgn::gltf::MaterialTehnique::LAMBERT;
		case GLTF::MaterialCommon::Technique::PHONG:
			return dgn::gltf::MaterialTehnique::PHONG;
		case GLTF::MaterialCommon::Technique::UNKNOWN:
			return dgn::gltf::MaterialTehnique::UNKNOWN;
		default:
			throw std::runtime_error("Unexpected case");
	}
}

/**Техника отрисовки из Java**/
GLTF::MaterialCommon::Technique	 GLTF::Utils::tehniqueJava2Gltf(dgn::gltf::MaterialTehnique java){
	switch (java){
		case dgn::gltf::MaterialTehnique::BLINN:
			return GLTF::MaterialCommon::Technique::BLINN;
		case dgn::gltf::MaterialTehnique::CONSTANT:
			return GLTF::MaterialCommon::Technique::CONSTANT;
		case dgn::gltf::MaterialTehnique::LAMBERT:
			return GLTF::MaterialCommon::Technique::LAMBERT;
		case dgn::gltf::MaterialTehnique::PHONG:
			return GLTF::MaterialCommon::Technique::PHONG;
		case dgn::gltf::MaterialTehnique::UNKNOWN:
			return GLTF::MaterialCommon::Technique::UNKNOWN;
		default:
			throw std::runtime_error("Unexpected case");
	}
}

/**Цвет из массива 4-х элементов**/
std::optional<dgn::gltf::Color> GLTF::Utils::colorArray2Java(float *vec4){
	return vec4 ? std::optional(
					dgn::gltf::Color{
									vec4[0], vec4[1], vec4[2], vec4[3]
					}):
	       std::nullopt;
}

/**Цвет в массив из 4-х элементов**/
float *GLTF::Utils::colorJava2Array(const std::optional<dgn::gltf::Color> color){
	if(color){
		float *vec = new float[4];
		vec[0] = color.value().red;
		vec[1] = color.value().green;
		vec[2] = color.value().blue;
		vec[3] = color.value().alpha;
		return vec;
	}else{
		return NULL;
	}
}