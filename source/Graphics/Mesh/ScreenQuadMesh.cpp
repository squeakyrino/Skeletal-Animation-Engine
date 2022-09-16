#include <Graphics/Mesh/ScreenQuadMesh.h>
#include <Graphics/OpenGLUtils.h>

namespace Engine
{
	void ScreenQuadMesh::createBuffers()
	{
		bindVAO();

		//Pos and tex coords
		int numVBOs = 2;

		std::unique_ptr<GLuint[]> vboIdDsBuffer(new GLuint[numVBOs]);

		glGenBuffers(numVBOs, vboIdDsBuffer.get());

		positions = {
			{ -1.0f,  1.0f },
			{ -1.0f, -1.0f },
			{ 1.0f,	  1.0f },
			{ 1.0f,  -1.0f }
		};

		std::vector<Vec2> texCoords = {			
			{ 0.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f }
		};

		indices = {
			0, 1, 2, 1, 3, 2
		};

		//Configure the vertex atributes

		//0 - Vertex pos
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[0], 0, positions);

		//2 - Tex coords
		OpenGLUtils::createVertexAttributes(vboIdDsBuffer[1], 2, texCoords);

		//Indices
		unsigned int eboID;
		glGenBuffers(1, &eboID);

		OpenGLUtils::createIndices(eboID, indices);

		unbindVAO();
	}


	ScreenQuadMesh::ScreenQuadMesh() :
		BaseMesh()
	{
		createBuffers();
	}

	size_t ScreenQuadMesh::getNumberVerts()
	{
		return positions.size();
	}

	size_t ScreenQuadMesh::getSizeIndices()
	{
		return indices.size();
	}
}