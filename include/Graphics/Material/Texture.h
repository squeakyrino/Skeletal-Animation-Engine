#pragma once
#include <string>

#include <GL/glew.h>

namespace Engine
{
	class Texture
	{
	private:

		unsigned int texObjID;

	public:
		//TODO: Expand stuff, make a base class, etc...
		Texture(const std::string& path, bool issRGB = false);

		//TODO: DEPTH TESTING
		Texture(int width, int height, bool depthOnly = false);

		Texture(int width, int height, GLenum internalFormat, GLenum format, GLenum type);

		unsigned int virtual getTexObjID();

		void resize(int width, int height);
	};
}