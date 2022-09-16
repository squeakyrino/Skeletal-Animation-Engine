#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color; 

//TODO: make an include preprocessor FML
//#include ./common/globalUniform.glsl

layout (std140) uniform GlobalMatrices
{
	uniform mat4 view;
	uniform mat4 projection;
};


uniform mat4 transform;

out vec3 oColor;

void main()
{
	
	oColor = color;
	
    gl_Position = projection * view * transform * vec4(pos, 1.0);
}