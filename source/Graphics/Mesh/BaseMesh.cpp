#include <Graphics/Mesh/BaseMesh.h>
#include <GL/glew.h>

namespace Engine
{
	BaseMesh::BaseMesh()
	{
		glGenVertexArrays(1, &vaoID);
	}
	
	void BaseMesh::bindVAO()
	{
		glBindVertexArray(vaoID);
	}
	
	void BaseMesh::unbindVAO()
	{
		glBindVertexArray(0);
	}
	
	unsigned int BaseMesh::getVAOID()
	{
		return vaoID;
	}
}

