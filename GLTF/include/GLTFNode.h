#pragma once

#include <set>
#include <string>
#include <vector>

#include "GLTFCamera.h"
#include "GLTFMesh.h"
#include "GLTFObject.h"
#include "GLTFSkin.h"
#include "dgn/gltf/GltfNode.hpp"

namespace GLTF {
	class Node : public GLTF::Object, public dgn::gltf::GltfNode {
	public:
		Node();
		~Node();
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

		//--------------------------------------------------------------------------------

		/**Идентификатор или имя объекта сцены */
		std::optional<std::string> getName();

		void setName(const std::optional<std::string> & nm);

		/**Дочерние узля сцены */
		std::vector<std::shared_ptr<GltfNode>> getChildren();

		void setChildren(const std::vector<std::shared_ptr<GltfNode>> & ch);

		/**Матрица преобразования 4x4 перечисление элементов по колонкам */
		std::optional<std::vector<float>> getMatrix();

		void setMatrix(const std::vector<float> & m);

		/**Меш связанный с  данным узлом сцены */
		std::shared_ptr<dgn::gltf::GltfMesh> getMesh();

		void setMesh(const std::shared_ptr<dgn::gltf::GltfMesh> & mesh);
	};
}
