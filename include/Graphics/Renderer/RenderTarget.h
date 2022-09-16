#pragma once
#include <Graphics/Material/Texture.h>

#include <memory>
#include <vector>

#include <GL/glew.h>


namespace Engine
{
	//TODO: Actually not so scuffed.
	//Works but it's not extremely flexible /shrug
	class RenderTarget
	{
	private:
		unsigned int fboId;

		unsigned int width;
		unsigned int height;
		
	public:

		std::vector<std::unique_ptr<Texture>> textures;
	public:

		
		RenderTarget(unsigned int width, unsigned height, bool depthOnly = false);
		
		RenderTarget(unsigned int width, unsigned height, GLenum type, unsigned int nrColorAttachments, bool depth);

		void bind();

		unsigned int getWidth();
		unsigned int getHeight();

	};
}