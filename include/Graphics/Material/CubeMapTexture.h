#pragma once
#include <Graphics/Material/TextureBase.h>

namespace Engine
{
	class CubeMapTexture : public TextureBase
	{
	private:

	public:
		//TODO: Lole. Better this
		CubeMapTexture(unsigned int width, unsigned int height, bool depthOnly);

	};
}