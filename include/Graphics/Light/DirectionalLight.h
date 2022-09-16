#pragma once
#include <Vectors\Vec3.h>
#include <Graphics/Light/BaseLight.h>

namespace Engine
{
	class DirectionalLight : public BaseLight
	{
	public:
		Vec3 direction;

	public:
		DirectionalLight(Vec3 direction, Vec3 ambient, Vec3 diffuse, Vec3 specular);
	};
}