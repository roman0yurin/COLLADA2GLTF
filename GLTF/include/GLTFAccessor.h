#pragma once

#include <string>

#include "GLTFBufferView.h"
#include "GLTFConstants.h"
#include "GLTFObject.h"
#include "dgn/gltf/GltfAccessor.hpp"
#include "dgn/gltf/GltfDataStruct.hpp"

namespace GLTF {
	class Accessor : public GLTF::Object, public dgn::gltf::GltfAccessor {
	public:
		enum class Type {
			SCALAR,
			VEC2,
			VEC3,
			VEC4,
			MAT2,
			MAT3,
			MAT4,
			UNKNOWN
		};

		std::shared_ptr<GLTF::BufferView> bufferView;
		int byteOffset = 0;
		GLTF::Constants::WebGL componentType;
		int count = 0;
		std::unique_ptr<float[]> max = NULL;
		std::unique_ptr<float[]> min = NULL;
		Type type = Type::UNKNOWN;

		Accessor(GLTF::Accessor::Type type, GLTF::Constants::WebGL componentType);

		/**
		 * Копиует параметр data, собственником остается тот, кто его передал
		 */
		Accessor(GLTF::Accessor::Type type,
			GLTF::Constants::WebGL componentType,
			unsigned char* data,
			int count,
			GLTF::Constants::WebGL target
		);

		Accessor(GLTF::Accessor::Type type,
			GLTF::Constants::WebGL componentType,
			unsigned char* data,
			int count,
			std::shared_ptr<GLTF::BufferView> bufferView
		);

		Accessor(GLTF::Accessor::Type type,
			GLTF::Constants::WebGL componentType,
			int byteOffset,
			int count,
		    std::shared_ptr<GLTF::BufferView> bufferView
		);

		static int getComponentByteLength(GLTF::Constants::WebGL componentType);
		static int getNumberOfComponents(GLTF::Accessor::Type type);

		bool computeMinMax();
		int getByteStride();
		bool getComponentAtIndex(int index, float *component);
		bool writeComponentAtIndex(int index, float* component);
		int getComponentByteLength();
		int getNumberOfComponents();
		bool equals(GLTF::Accessor* accessor);
		const char* getTypeName();

		virtual std::string typeName();
		virtual void writeJSON(void* writer, GLTF::Options* options);

		/**Область даных */
		std::shared_ptr<dgn::gltf::GltfBufferView> getBufferView();

		void setBufferView(const std::shared_ptr<dgn::gltf::GltfBufferView> & view);

		/**Сдвиг внутри области данных */
		int32_t getByteOffset();

		void setByteOffset(int32_t value);

		/**Количество элементов GltfDataStruct (не байт) */
		int32_t getCount();

		virtual void setCount(int32_t value);

		/**Структура данных */
		dgn::gltf::GltfDataStruct getStruct();

		void setStruct(dgn::gltf::GltfDataStruct s);
	};
};
