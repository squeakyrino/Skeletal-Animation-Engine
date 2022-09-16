#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 2) in ivec4 jointIds;
layout (location = 3) in vec4 jointWeights; 

uniform mat4 transform;
uniform mat4 lightProjView;

const int MAX_BONES = 200;

uniform mat4 boneTransform[MAX_BONES];

void main()
{
    mat4 meshSpace = boneTransform[jointIds[0]] * jointWeights[0];
	meshSpace += boneTransform[jointIds[1]] * jointWeights[1];
	meshSpace += boneTransform[jointIds[2]] * jointWeights[2];
	meshSpace += boneTransform[jointIds[3]] * jointWeights[3];

    gl_Position = lightProjView * transform * meshSpace * vec4(pos, 1.0);
}  