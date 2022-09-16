#pragma once
#include <Vectors\Vec3.h>
#include <Graphics/Light/BaseLight.h>

namespace Engine
{
	class PointLight : public BaseLight
	{
	private:

		void calculateFactorsFromRange(const float& range);

	public:
		Vec3 position;
		float linearFac;
		float quadraticFac;

		PointLight(Vec3 position, float range, Vec3 ambient, Vec3 diffuse, Vec3 specular);
	};
}