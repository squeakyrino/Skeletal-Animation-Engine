#pragma once
#include <Vectors\Vec2.h>
#include <Vectors\Vec3.h>
#include <Vectors\Vec4.h>

#include <vector>

#include <GL/glew.h>

namespace Engine
{
	class OpenGLUtils
	{
	public:
		static void createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec2>& values);
		static void createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec3>& values);
		static void createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, const std::vector<Vec4>& values);
		static void createVertexAttributes(const unsigned int& vboID, const unsigned int& attribNumber, 
			std::vector<std::vector<unsigned int>>& values);

		static void createIndices(const unsigned int& eboID, const std::vector<unsigned int>& indices);

		static std::vector<GLfloat> vecToOpenGLVec(const std::vector<Vec2>& v);
		static std::vector<GLfloat> vecToOpenGLVec(const std::vector<Vec3>& v);
		static std::vector<GLfloat> vecToOpenGLVec(const std::vector<Vec4>& v);
	};
}