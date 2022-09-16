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

out VS_OUT
{
    vec3 oColor;
	vec2 otexCoords;
	vec3 o_Normal;
	vec3 o_FragPos;
	mat3 o_TBN;
} vs_out;

void main()
{	
	mat4 finalTransform = transform;
	
	//TBN
	vec3 T = normalize(vec3(finalTransform * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(finalTransform * vec4(normal,    0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);
	
	
	//Outs
	vs_out.oColor = color;
	vs_out.otexCoords = texCoords;
	vs_out.o_Normal = mat3(transpose(inverse(finalTransform))) * normal;
	vs_out.o_FragPos = vec3(finalTransform * vec4(pos, 1.0));
	vs_out.o_TBN = TBN;
	
	//Output
	gl_Position = projection * view * finalTransform * vec4(pos, 1.0);
}