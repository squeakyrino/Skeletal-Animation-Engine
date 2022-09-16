#version 330 core

layout (location = 0) in vec3 pos;

//Include global matrices
#include ../common/globalUniform.glsl //?#include "../common/globalUniform.glsl"

uniform mat4 transform;
uniform mat4 lightView;
uniform mat4 lightProj;

out VS_OUT
{
    vec4 o_fragPosLightSpace;
} vs_out;

void main()
{	
	mat4 finalTransform = transform;

	vec3 fragPos = vec3(finalTransform * vec4(pos, 1.0));
	
	//Outs
	vs_out.o_fragPosLightSpace = lightProj * lightView * vec4(fragPos, 1.0);

	//Output
	gl_Position = projection * view * finalTransform * vec4(pos, 1.0);
}