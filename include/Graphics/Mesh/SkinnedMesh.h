#pragma once
#include <Vectors\Vec3.h>
#include <Graphics\Animation\Joint.h>
#include <Graphics/Mesh/BaseMesh.h>
#include <Engine Systems/ModelLoader.h>

#include <vector>

namespace Engine
{
	class SkinnedMesh : public BaseMesh
	{
	private:

		//Only the numbers of vertices and the number of indices are necessary. 
		//Vertex data
		std::vector<Vec3> positions;
		std::vector<unsigned int> indices;

		//Create the opengl buffers and send the data over to the GPU
		void CreateBuffers(const std::vector<Vec3>& positions,
			const std::vector<Vec3>& colors,
			const std::vector<unsigned int>& indices,
			std::vector<std::vector<unsigned int>>& boneIDs,
			const std::vector<Vec4>& weights,
			const std::vector<Vec2>& texCoords,
			const std::vector<Vec3>& normals, 
			const std::vector<Vec3>& tangents, 
			const std::vector<Vec3>& bitangents);

	public:

		//Pass ModelLoader because model loader does more things than just loading in
		//Vertices
		SkinnedMesh(const ModelLoader& modelLoader, const Vec3& color);

		//Skeleton data
		//TODO: This should be a pointer but ugh
		//Joint rootJoint;
		unsigned int jointCount;

		std::vector<Mat4> getJointTransforms();

		// Inherited via BaseMesh
		virtual size_t getNumberVerts() override;
		virtual size_t getSizeIndices() override;
	};
}