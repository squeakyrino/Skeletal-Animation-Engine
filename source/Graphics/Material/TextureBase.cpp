#include <Graphics/Material/TextureBase.h>
#include <GL\glew.h>

namespace Engine
{
	TextureBase::TextureBase()
	{
		glGenTextures(1, &texObjID);
	}

	unsigned int TextureBase::getTexObjID()
	{
		return texObjID;
	}
}
