#pragma once

namespace Engine
{
	class TextureBase
	{
	protected:
		unsigned int texObjID;

	public:

		TextureBase();

		unsigned int virtual getTexObjID();


	};
}