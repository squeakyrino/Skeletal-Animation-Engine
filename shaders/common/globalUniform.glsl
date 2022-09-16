#ifndef UNIFORMS_H
#define UNIFORMS_H
#include ./lightDeclaration.glsl //? #include "./lightDeclaration.glsl"

layout (std140) uniform GlobalMatrices
{
	uniform mat4 view;
	uniform mat4 projection;
	vec4 camPos;
	DirectionalLight dirLights[MAX_DIR_LIGHTS];	
	PointLight pointLights[MAX_POINT_LIGHTS];
};
#endif