#include <Graphics/Light/DirectionalLight.h>

namespace Engine
{
	DirectionalLight::DirectionalLight(Vec3 direction, Vec3 ambient, Vec3 diffuse, Vec3 specular) :
		BaseLight(ambient, diffuse, specular), direction(direction.normalize())
	{
	}
}