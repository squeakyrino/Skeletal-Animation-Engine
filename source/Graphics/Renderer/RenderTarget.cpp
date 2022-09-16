#include <Graphics/Renderer/RenderTarget.h>

#include <GL\glew.h>
#include <Graphics\Material\Texture.h>
#include <iostream>

namespace Engine
{
	RenderTarget::RenderTarget(unsigned int width, unsigned height, bool depthOnly) :
		width(width), height(height)
	{
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		//


		//tex = std::make_unique<Texture>(width, height, depthOnly);

		textures.emplace_back(new Texture(width, height, depthOnly));

		if (depthOnly)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0]->getTexObjID(), 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

		}
		else
		{
			//Attach texture
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0]->getTexObjID(), 0);

			//TODO: Creating a render buffer for testing. Just for testing!!!!
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);


			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		}
		

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			exit(EXIT_FAILURE);
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	RenderTarget::RenderTarget(unsigned int width, unsigned height, GLenum type, unsigned int nrColorAttachments, bool depth) :
		width(width), height(height)
	{
		//Generate the buffer
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);


		for (unsigned int i = 0; i < nrColorAttachments; i++)
		{
			//Generate a texture for each color attachment
			
			//Assume it's a regular texture if the type is UNSIGNED_BYTE
			GLenum internalFormat = GL_RGBA;

			//If we define the type to be a floating point texture then change it
			if (type == GL_FLOAT)
				internalFormat = GL_RGBA32F;

			textures.emplace_back(new Texture(width, height, internalFormat, GL_RGBA, type));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i]->getTexObjID(), 0);
		}

		if (depth)
		{
			//TODO: No stencil for now because fuck you
			//Also don't know if I should use a depth texture or a framebuffer works fine just for
			//depth testing

			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			exit(EXIT_FAILURE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTarget::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	}
	
	unsigned int RenderTarget::getWidth()
	{
		return width;
	}
	
	unsigned int RenderTarget::getHeight()
	{
		return height;
	}
}