#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;  
layout (location = 2) in vec2 texCoords; 
layout (location = 3) in vec3 normal; 
layout (location = 4) in vec3 tangent; 
layout (location = 5) in vec3 bitangent; 

//Include global matrices
#include ./common/globalUniform.glsl

uniform mat4 transform;

out vec3 out_color;

void main(void)
{
	gl_Position = projection * view * transform * vec4(pos, 1.0);

	out_color = color;
}