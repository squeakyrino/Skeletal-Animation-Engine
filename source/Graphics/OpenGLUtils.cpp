#include <Graphics/OpenGLUtils.h>

namespace Engine
{

	void OpenGLUtils::createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec2>& values)
	{
		//Vec2
		int numComponents = 2;

		std::vector<GLfloat> data = vecToOpenGLVec(values);

		//2 - bind the first vbo to the vao
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//2.1 - send the data over
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		//2.2 - set the pointers
		glVertexAttribPointer(attribNumber, numComponents, GL_FLOAT, GL_FALSE, numComponents * sizeof(GLfloat), (GLvoid*)0);
		//2.3 - enable the attrib
		glEnableVertexAttribArray(attribNumber);
	}
	
	void OpenGLUtils::createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec3>& values)
	{
		//Vec3
		int numComponents = 3;

		std::vector<GLfloat> data = vecToOpenGLVec(values);

		//2 - bind the first vbo to the vao
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//2.1 - send the data over
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		//2.2 - set the pointers
		glVertexAttribPointer(attribNumber, numComponents, GL_FLOAT, GL_FALSE, numComponents * sizeof(GLfloat), (GLvoid*)0);
		//2.3 - enable the attrib
		glEnableVertexAttribArray(attribNumber);
	}

	void OpenGLUtils::createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec4>& values)
	{
		//Vec4
		int numComponents = 4;

		std::vector<GLfloat> data = vecToOpenGLVec(values);

		//2 - bind the first vbo to the vao
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		//2.1 - send the data over
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
		//2.2 - set the pointers
		glVertexAttribPointer(attribNumber, numComponents, GL_FLOAT, GL_FALSE, numComponents * sizeof(GLfloat), (GLvoid*)0);
		//2.3 - enable the attrib
		glEnableVertexAttribArray(attribNumber);
	}

	void OpenGLUtils::createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber,
		std::vector<std::vector<unsigned int>>& values) 
	{
		//This only allows an array of 4 ints
		int numComponents = 4;

		std::vector<unsigned int> data;

		for (int i = 0; i < values.size(); i++)
		{
			//NOTE: all vertex have max 4 joints so this is ok
			for (int j = 0; j < values[0].size(); j++)
			{
				data.push_back(values[i][j]);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLint), data.data(), GL_STATIC_DRAW);
		glVertexAttribIPointer(attribNumber, numComponents, GL_INT, numComponents * sizeof(GLint), (GLvoid*)0);
		glEnableVertexAttribArray(attribNumber);
	}

	void OpenGLUtils::createIndices(const unsigned int& eboID, const std::vector<unsigned int>& indices)
	{
		//If indices are specified send them over
		if (indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		}
	}

	std::vector<GLfloat> OpenGLUtils::vecToOpenGLVec(const std::vector<Vec2>& v)
	{
		std::vector<GLfloat> result;
		for (auto& value : v)
		{
			result.push_back((GLfloat)value.x);
			result.push_back((GLfloat)value.y);
		}

		return result;
	}

	std::vector<GLfloat> OpenGLUtils::vecToOpenGLVec(const std::vector<Vec3>& v)
	{
		std::vector<GLfloat> result;
		for (auto& value : v)
		{
			result.push_back((GLfloat)value.x);
			result.push_back((GLfloat)value.y);
			result.push_back((GLfloat)value.z);
		}

		return result;
	}

	std::vector<GLfloat> OpenGLUtils::vecToOpenGLVec(const std::vector<Vec4>& v)
	{
		std::vector<GLfloat> result;
		for (auto& value : v)
		{
			result.push_back((GLfloat)value.x);
			result.push_back((GLfloat)value.y);
			result.push_back((GLfloat)value.z);
			result.push_back((GLfloat)value.w);
		}

		return result;
	}
}