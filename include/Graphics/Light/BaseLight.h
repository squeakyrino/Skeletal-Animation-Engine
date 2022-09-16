#pragma once
#include <Vectors\Vec3.h>

namespace Engine
{
	class BaseLight
	{
	public:
		Vec3 ambient;
		Vec3 diffuse;
		Vec3 specular;

	public:
		BaseLight(Vec3 ambient,	Vec3 diffuse, Vec3 specular);
	};
}