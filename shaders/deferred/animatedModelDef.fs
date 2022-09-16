#version 330 core

#include ../common/materialProperties.glsl //? #include "../common/materialProperties.glsl"

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gTBNNormal;
layout (location = 2) out vec3 gTexColor;
layout (location = 3) out vec3 gMatAmb;
layout (location = 4) out vec3 gMatDiff;
layout (location = 5) out vec4 gMatSpecAndShininess;

uniform Material mat;

in VS_OUT
{
    vec3 oColor;
	vec2 otexCoords;
	vec3 o_Normal;
	vec3 o_FragPos;
	mat3 o_TBN;
} fs_in;

uniform sampler2D texMap;
uniform sampler2D normalMap;

void main()
{
	//Put stuff in the gbuffer
	//Position
	gPos = fs_in.o_FragPos;

	//Normal in [0, 1] range
	vec3 normal = texture(normalMap, fs_in.otexCoords).xyz;
	//Convert to range [-1, 1] 
	normal = normal * 2.0 - 1.0; 
	//(in world space)
	gTBNNormal = normalize(fs_in.o_TBN * normal);

	//Tex color
	gTexColor = texture(texMap, fs_in.otexCoords).rgb;

	//Ambient color
	gMatAmb = mat.ambient;

	//Diffuse
	gMatDiff = mat.diffuse;

	//Specular and shininess
	gMatSpecAndShininess.rgb = mat.specular;
	gMatSpecAndShininess.a = mat.shininess;

}