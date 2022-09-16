#pragma once
#include <GL/glew.h>

namespace Engine
{
	class UniformSamplerValue
	{
	protected:
		unsigned int texObjID;
		GLenum texTarget;
		unsigned int texUnit;

	public:
		UniformSamplerValue(int texObjId, GLenum texTarget, unsigned int texUnit)
			: texObjID(texObjId), texTarget(texTarget), texUnit(texUnit) {}

		virtual void setUniformSampler()
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(texTarget, texObjID);
		};
	};

	class UniformSamplerValue2D : public UniformSamplerValue
	{

	public:
		UniformSamplerValue2D(int texObjId, unsigned int texUnit) :
			UniformSamplerValue(texObjId, GL_TEXTURE_2D, texUnit) {}
	};
}