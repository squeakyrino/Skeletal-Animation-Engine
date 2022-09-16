#include <Graphics/Light/BaseLight.h>

namespace Engine
{
	BaseLight::BaseLight(Vec3 ambient, Vec3 diffuse, Vec3 specular) :
		ambient(ambient), diffuse(diffuse), specular(specular)
	{
	}
}
