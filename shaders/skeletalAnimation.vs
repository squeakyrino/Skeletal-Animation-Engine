#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color; 
layout (location = 2) in ivec4 jointIds;
layout (location = 3) in vec4 jointWeights; 

//TODO: make an include preprocessor FML
//#include ./common/globalUniform.glsl

layout (std140) uniform GlobalMatrices
{
	uniform mat4 view;
	uniform mat4 projection;
};

const int MAX_BONES = 200;

uniform mat4 transform;
uniform mat4 boneTransform[MAX_BONES];

uniform int mode;
uniform int numBones;

out vec3 oColor;

vec3 colorByIds()
{
	vec3 outputColor = vec3(0.0);
	
	//Add one to the bones so ID 0 isn't completely black
	//This makes it so the last bones technically go overboard but eh
	//It's just debug info anyway
	outputColor.x = float(jointIds.x + 1);
	outputColor.y = float(jointIds.y + 1);
	outputColor.z = float(jointIds.z + 1);
		
	//Divide by the number of bones
	outputColor = outputColor / float(numBones);
	
	return outputColor;
}

vec3 colorByWeights()
{	
	return vec3(jointWeights);
}

void main()
{	
	//With skinning info
	mat4 meshSpace = boneTransform[jointIds[0]] * jointWeights[0];
	meshSpace += boneTransform[jointIds[1]] * jointWeights[1];
	meshSpace += boneTransform[jointIds[2]] * jointWeights[2];
	meshSpace += boneTransform[jointIds[3]] * jointWeights[3];
	
    gl_Position = projection * view * transform * meshSpace * vec4(pos, 1.0);
	
	//Calculate color using jointIds or weights!
	if(mode == 0)
	{
		oColor = colorByIds();
	}
	else
	{
		oColor = colorByWeights();
	}
}