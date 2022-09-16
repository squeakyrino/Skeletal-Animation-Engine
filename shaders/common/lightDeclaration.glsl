#ifndef LIGHT_H
#define LIGHT_H

const int MAX_DIR_LIGHTS = 4;
const int MAX_POINT_LIGHTS = 8;

struct DirectionalLight
{
	vec4 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct PointLight
{
	vec4 position;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float linear;
    float quadratic;
};
#endif


