#include <Graphics/Light/PointLight.h>

namespace Engine
{
	void PointLight::calculateFactorsFromRange(const float& range)
	{
		//Taken from http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		//And https://learnopengl.com/Lighting/Light-casters

		if (range < 7)
		{
			this->linearFac = 0.7f;
			this->quadraticFac = 1.8f;
		}
		else if (range < 13)
		{
			this->linearFac = 0.35f;
			this->quadraticFac = 0.44f;
		}
		else if (range < 20)
		{
			this->linearFac = 0.22f;
			this->quadraticFac = 0.20f;
		}
		else if (range < 32)
		{
			this->linearFac = 0.14f;
			this->quadraticFac = 0.07f;
		}
		else if (range < 50)
		{
			this->linearFac = 0.09f;
			this->quadraticFac = 0.032f;
		}
		else if (range < 65)
		{
			this->linearFac = 0.07f;
			this->quadraticFac = 0.017f;
		}
		else if (range < 100)
		{
			this->linearFac = 0.045f;
			this->quadraticFac = 0.0075f;
		}
		else if (range < 160)
		{
			this->linearFac = 0.027f;
			this->quadraticFac = 0.0028f;
		}
		else if (range < 200)
		{
			this->linearFac = 0.022f;
			this->quadraticFac = 0.0019f;
		}
		else if (range < 325)
		{
			this->linearFac = 0.014f;
			this->quadraticFac = 0.0007f;
		}
		else if (range < 600)
		{
			this->linearFac = 0.007f;
			this->quadraticFac = 0.0002f;
		}
		else
		{
			this->linearFac = 0.0014f;
			this->quadraticFac = 0.000007f;
		}
	}

	PointLight::PointLight(Vec3 position, float range, Vec3 ambient, Vec3 diffuse, Vec3 specular) :
		BaseLight(ambient, diffuse, specular), position(position)
	{
		calculateFactorsFromRange(range);
	}
}