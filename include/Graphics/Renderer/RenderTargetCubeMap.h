#pragma once
#include <Graphics\Material\CubeMapTexture.h>

#include <vector>
#include <memory>

namespace Engine
{
	class RenderTargetCubeMap
	{
	private:
		unsigned int fboId;

		unsigned int width;
		unsigned int height;

	public:

		std::vector<std::unique_ptr<CubeMapTexture>> textures;

		RenderTargetCubeMap(unsigned int width, unsigned height, bool depthOnly = false);

		//TODO: Base class lole
		void bind();

		unsigned int getWidth();
		unsigned int getHeight();
	};
}