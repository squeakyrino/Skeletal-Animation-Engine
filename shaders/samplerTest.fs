#version 330 core

#include ./common/globalUniform.glsl //? #include "./common/globalUniform.glsl"
#include ./common/lightFunctions.glsl //? #include "./common/lightFunctions.glsl"
#include ./common/materialProperties.glsl //? #include "./common/materialProperties.glsl"
#include ./common/gammaCorrection.glsl //? #include "./common/gammaCorrection.glsl"

in VS_OUT
{
    vec3 oColor;
	vec2 otexCoords;
	vec3 o_Normal;
	vec3 o_FragPos;
	mat3 o_TBN;
} fs_in;

uniform Material mat;

out vec4 FragColor;

uniform sampler2D textTest;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float width;
uniform float height;

void main()
{
	//Normal used for light calculation

	/*TODO: make this into a function: Normal from normal map*/
	//Range [0, 1]	
	vec3 normal = texture(normalMap, fs_in.otexCoords).xyz;
	//Range [-1, 1]
	normal = normal * 2.0 - 1.0; 
	normal = normalize(fs_in.o_TBN * normal);
	/**/
	
	vec3 texColor = texture(textTest, fs_in.otexCoords).rgb;
	vec3 viewDir = normalize(camPos.xyz - fs_in.o_FragPos);

	//Result
	vec3 outputColor = vec3(0);

	//TODO: TESTING SHADOWS
	// Retrieve the shadow color from shadow map
	vec2 c = gl_FragCoord.xy;
	c.x /= width;
	c.y /= height;
	
	float shadow = texture(depthMap, c).r;

	//Directional lights
	for(int i = 0; i < MAX_DIR_LIGHTS; i++)
	{
		outputColor += CalcDirLight(dirLights[i], mat, texColor, normal, viewDir, shadow);
	}
	
	//Point lights
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		outputColor += CalcPointLight(pointLights[i], mat, texColor, normal,
					fs_in.o_FragPos, viewDir, shadow);
	}
	
	
	//Apply gamma correction
	outputColor = applyGammaCorrection(outputColor);

	FragColor = vec4(outputColor, 1);
	
	//FragColor = vec4(texture(depthMap, c).rgb, 1);

	//TODO: Different view modes maybe??
	//With a uniform

	//FragColor = texture(normalMap, fs_in.otexCoords);
	
	//FragColor = vec4(fs_in.otexCoords, 0, 1);
	//FragColor = vec4(fs_in.oColor, 1);
	
	//FragColor = vec4(normal, 1);

	/*Visualize depth buffer* /
	float ndc = gl_FragCoord.z * 2.0 - 1.0; 
	float near = 0.001;
	float far = 100;

	float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near)) / far;	
	FragColor = vec4(vec3(linearDepth), 1.0);
	/**/
}