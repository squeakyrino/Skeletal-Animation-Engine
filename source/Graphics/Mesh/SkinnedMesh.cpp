#include <Graphics/Mesh/SkinnedMesh.h>
#include <Engine Systems\ModelLoader.h>
#include <Graphics\OpenGLUtils.h>

#include <GL/glew.h>


namespace Engine
{
	void SkinnedMesh::CreateBuffers(const std::vector<Vec3>& positions,
	const std::vector<Vec3>& colors,
	const std::vector<unsigned int>& indices,
	std::vector<std::vector<unsigned int>>& boneIDs,
	const std::vector<Vec4>& weights,
	const std::vector<Vec2>& texCoords,
	const std::vector<Vec3>& normals,
	const std::vector<Vec3>& tangents,
	const std::vector<Vec3>& bitangents)
	{
		//Bind VAO
		bindVAO();
		//And the corresponding VBOs
		
		int numVBOs = 8;
		
		std::unique_ptr<GLuint[]> vboIdDsBuffer(new GLuint[numVBOs]);

		glGenBuffers(numVBOs, vboIdDsBuffer.get());

		//If indices are specified create ebo
		unsigned int eboID;
		if (indices.size() > 0)
		{
			glGenBuffers(1, &eboID);
		}

		//Configure the vertex atributes

		//1 - Vertex pos
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[0], 0, positions);

		//2 - Colors
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[1], 1, colors);

		//3 - Vertex joints ID
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[2], 2, boneIDs);

		//4 - Bone weights
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[3], 3, weights);

		//5 - Tex coords
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[4], 4, texCoords);

		//6 - Normals
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[5], 5, normals);

		//7 - Tangent
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[6], 6, tangents);

		//8 - Bitangent
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[7], 7, bitangents);

		OpenGLUtils::createIndices(eboID, indices);

		//Unbind current VAO
		unbindVAO();
	}

	SkinnedMesh::SkinnedMesh(const ModelLoader& modelLoader, const Vec3& color) :
		BaseMesh()
	{
		positions = modelLoader.getVertices();
		std::vector<Vec3> colors = std::vector<Vec3>(modelLoader.getVertices().size(), color);
		indices = modelLoader.getIndices();
		
		std::vector<std::vector<unsigned int>> boneIDs = modelLoader.getBoneIds();
		std::vector<Vec4> weights = modelLoader.getBoneWeights();

		std::vector<Vec2> texCoords = modelLoader.getTexCoords();

		std::vector<Vec3> normals = modelLoader.getNormals();
		std::vector<Vec3> tangent = modelLoader.getTangents();
		std::vector<Vec3> bitangent = modelLoader.getBitTangents();

		CreateBuffers(positions, colors, indices, boneIDs, weights, texCoords, normals, tangent, bitangent);
		
		//TODO: We don't actually get the root joint from here to pass it to the animator because I'm
		//Bad. Refactor later into making joints points instead of stack memory
		//rootJoint = modelLoader.getRootJoint();
		jointCount = modelLoader.getNumBones();
	}

	std::vector<Mat4> SkinnedMesh::getJointTransforms()
	{
		return std::vector<Mat4>();
	}

	size_t SkinnedMesh::getNumberVerts()
	{
		return positions.size();
	}

	size_t SkinnedMesh::getSizeIndices()
	{
		return indices.size();
	}
}