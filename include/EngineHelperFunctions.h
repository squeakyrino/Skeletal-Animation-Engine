#pragma once

#include <EngineConstants.h>

namespace Engine
{
	static float toRad(float degrees)
	{
		return degrees * PI / 180.0f;
	}

	static float lerpf(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static std::string getDirectoryFromPath(const std::string& fname)
	{
		size_t pos = fname.find_last_of("\\/");
		return (std::string::npos == pos)
			? ""
			: fname.substr(0, pos);
	}
}