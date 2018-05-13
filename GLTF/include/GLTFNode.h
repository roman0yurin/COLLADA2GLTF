#pragma once

#include <set>
#include <string>
#include <vector>

#include "GLTFCamera.h"
#include "GLTFMesh.h"
#include "GLTFObject.h"
#include "GLTFSkin.h"

namespace GLTF {
	class Node : public GLTF::Object {
	public:
		Node();
		class Transform : public std::enable_shared_from_this<Transform>{
		public:
			enum Type {
				TRS,
				MATRIX
			};

			Type type;
		};

		class TransformTRS;
		class TransformMatrix : public Transform {
		public:
			float matrix[16];

			TransformMatrix();
			TransformMatrix(float a00, float a01, float a02, float a03, 
				float a10, float a11, float a12, float a13, 
				float a20, float a21, float a22, float a23, 
				float a30, float a31, float a32, float a33);

			void premultiply(std::shared_ptr<TransformMatrix> transform);
			void premultiply(std::shared_ptr<TransformMatrix> transform, std::shared_ptr<TransformMatrix> destination);
			void scaleUniform(float scale);
			bool isIdentity();
			void getTransformTRS(std::shared_ptr<TransformTRS> out);
			std::shared_ptr<TransformTRS> getTransformTRS();
		};

		class TransformTRS : public Transform {
		public:
			float translation[3];
			float rotation[4];
			float scale[3];

			TransformTRS();
			bool isIdentityTranslation();
			bool isIdentityRotation();
			bool isIdentityScale();
			std::shared_ptr<TransformMatrix> getTransformMatrix();
		};

		std::shared_ptr<GLTF::Camera> camera;
		std::vector<std::shared_ptr<GLTF::Node>> children;
		std::shared_ptr<GLTF::Skin> skin;
		std::string jointName;
		std::shared_ptr<GLTF::Mesh> mesh;
		std::shared_ptr<GLTF::MaterialCommon::Light> light;

		std::shared_ptr<Transform> transform ;

		virtual std::string typeName();
		virtual std::shared_ptr<GLTF::Object> clone(std::shared_ptr<GLTF::Object> clone);
		virtual void writeJSON(void* writer, GLTF::Options* options);
	};
}
