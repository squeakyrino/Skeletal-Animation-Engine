#include <Graphics/Renderer/RenderTargetCubeMap.h>

#include <iostream>

#include <GL\glew.h>
namespace Engine
{
	RenderTargetCubeMap::RenderTargetCubeMap(unsigned int width, unsigned height, bool depthOnly)
		: width(width), height(height)
	{
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);

		textures.emplace_back(new CubeMapTexture(width, height, depthOnly));

		if (depthOnly)
		{
			//TODO: Assumes we render to the texture cube all at once. Maybe change later idk
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textures[0]->getTexObjID(), 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			exit(EXIT_FAILURE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTargetCubeMap::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	}
	
	unsigned int RenderTargetCubeMap::getWidth()
	{
		return width;
	}
	
	unsigned int RenderTargetCubeMap::getHeight()
	{
		return height;
	}
}
