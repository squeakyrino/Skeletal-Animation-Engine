#include <Graphics/Mesh/Mesh.h>
#include <Graphics/OpenGLUtils.h>
#include <Engine Systems/ModelLoader.h>

namespace Engine
{
	void Mesh::CreateBuffers(const std::vector<Vec3>& positions,
		const std::vector<Vec3>& colors,
		const std::vector<unsigned int>& indices,
		const std::vector<Vec2>& texCoords,
		const std::vector<Vec3>& normals,
		const std::vector<Vec3>& tangents,
		const std::vector<Vec3>& bitangents)
	{
		//Bind VAO
		bindVAO();
		//And the corresponding VBOs

		int numVBOs = 6;

		std::unique_ptr<GLuint[]> vboIdDsBuffer(new GLuint[numVBOs]);

		glGenBuffers(numVBOs, vboIdDsBuffer.get());

		//If indices are specified create ebo
		unsigned int eboID;
		if (indices.size() > 0)
		{
			glGenBuffers(1, &eboID);
		}

		//Configure the vertex atributes

		//0 - Vertex pos
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[0], 0, positions);

		//1 - Colors
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[1], 1, colors);

		//2 - Tex coords
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[2], 2, texCoords);

		//3 - Normals
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[3], 3, normals);

		//4 - Tangent
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[4], 4, tangents);

		//5 - Bitangent
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[5], 5, bitangents);

		OpenGLUtils::createIndices(eboID, indices);

		//Unbind current VAO
		unbindVAO();
	}

	Mesh::Mesh(const std::string& objPath, const Vec3& color, bool compress) :
		BaseMesh()
	{
		ModelLoader loader = ModelLoader(objPath);

		CreateBuffers(loader.getVertices(),
			std::vector<Vec3>(loader.getVertices().size(), color),
			loader.getIndices(),
			loader.getTexCoords(),
			loader.getNormals(),
			loader.getTangents(),
			loader.getBitTangents());

		this->positions = loader.getVertices();
		this->indices = loader.getIndices();
	}

	Mesh::Mesh(const std::string& objPath, const std::vector <Vec3>& colors, bool compress) :
		BaseMesh()
	{
		ModelLoader loader = ModelLoader(objPath);

		CreateBuffers(loader.getVertices(),
			colors,
			loader.getIndices(),
			loader.getTexCoords(),
			loader.getNormals(),
			loader.getTangents(),
			loader.getBitTangents());

		this->positions = loader.getVertices();
		this->indices = loader.getIndices();
	}

	Mesh::Mesh(const ModelLoader& modelLoader, const Vec3& color, bool compress)
	{
		CreateBuffers(modelLoader.getVertices(),
			std::vector<Vec3>(modelLoader.getVertices().size(), color),
			modelLoader.getIndices(),
			modelLoader.getTexCoords(),
			modelLoader.getNormals(),
			modelLoader.getTangents(),
			modelLoader.getBitTangents());

		this->positions = modelLoader.getVertices();
		this->indices = modelLoader.getIndices();
	}

	Mesh::Mesh(const ModelLoader& modelLoader, const std::vector<Vec3>& colors, bool compress)
	{
		CreateBuffers(modelLoader.getVertices(),
			colors,
			modelLoader.getIndices(),
			modelLoader.getTexCoords(),
			modelLoader.getNormals(),
			modelLoader.getTangents(),
			modelLoader.getBitTangents());

		this->positions = modelLoader.getVertices();
		this->indices = modelLoader.getIndices();
	}

	Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& colors, const std::vector<unsigned int>& indices) :
		BaseMesh()
	{
		CreateBuffers(positions,
			colors,
			indices,
			std::vector<Vec2>(),
			std::vector<Vec3>(),
			std::vector<Vec3>(),
			std::vector<Vec3>());

		this->positions = positions;
		this->indices = indices;
	}

	size_t Mesh::getNumberVerts()
	{
		return positions.size();
	}

	size_t Mesh::getSizeIndices()
	{
		return indices.size();
	}
}