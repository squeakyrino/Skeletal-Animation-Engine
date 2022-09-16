#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color; 
layout (location = 2) in ivec4 jointIds;
layout (location = 3) in vec4 jointWeights; 
layout (location = 4) in vec2 texCoords; 
layout (location = 5) in vec3 normal; 
layout (location = 6) in vec3 tangent; 
layout (location = 7) in vec3 bitangent; 

//Include global matrices
#include ../common/globalUniform.glsl //? #include "../common/globalUniform.glsl"

const int MAX_BONES = 200;

uniform mat4 transform;
uniform mat4 boneTransform[MAX_BONES];

uniform int mode;
uniform int numBones;

out VS_OUT
{
    vec3 oColor;
	vec2 otexCoords;
	vec3 o_Normal;
	vec3 o_FragPos;
	mat3 o_TBN;
} vs_out;

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
	
	mat4 finalTransform = transform * meshSpace;
	
	//TBN
	vec3 T = normalize(vec3(finalTransform * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(finalTransform * vec4(normal,    0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	
	//Calculate color using jointIds or weights!
	if(mode == 0)
	{
		vs_out.oColor = colorByIds();
	}
	else
	{
		vs_out.oColor = colorByWeights();
	}
	
	vs_out.otexCoords = texCoords;
	
	vs_out.o_Normal = mat3(transpose(inverse(finalTransform))) * normal;
	
	vs_out.o_FragPos = vec3(finalTransform * vec4(pos, 1.0));
	
	vs_out.oColor = tangent;
	
	vs_out.o_TBN = TBN;

	//Output
	gl_Position = projection * view * finalTransform * vec4(pos, 1.0);
}