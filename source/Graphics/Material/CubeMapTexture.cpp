#include <Graphics/Material/CubeMapTexture.h>
#include <GL\glew.h>
#include <iostream>

namespace Engine
{
	CubeMapTexture::CubeMapTexture(unsigned int width, unsigned int height, bool depthOnly)
		: TextureBase()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texObjID);

		if (depthOnly)
		{
			for (unsigned int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
					width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

		}
		else
		{
			//TODO: make it easier to pass in the format, like the render target does it
			std::cout << "CubeMapTexture not depth only" << std::endl;
			exit(EXIT_FAILURE);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
