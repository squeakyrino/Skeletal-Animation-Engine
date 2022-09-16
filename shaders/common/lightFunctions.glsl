#ifndef LIGHTFUNC_H
#define LIGHTFUNC_H

#include ./lightDeclaration.glsl //? #include "./lightDeclaration.glsl"
#include ./materialProperties.glsl //? #include "./materialProperties.glsl"

vec3 getHalfwayDirVec(vec3 lightDir, vec3 viewDir)
{
	return normalize(lightDir + viewDir);
}

vec3 calcAmbient(vec3 lightAmb, vec3 matAmb)
{
	return lightAmb * matAmb;
}

vec3 calcDiffuse(vec3 lightDiff, vec3 matDiff, float diffFac)
{
	return lightDiff * matDiff * diffFac;
}

vec3 calcSpecular(vec3 lightSpec, vec3 matSpec, float specFac)
{
	return lightSpec * matSpec * specFac;
}

float calcDiffFac(vec3 normal, vec3 lightDir)
{
	return max(dot(normal, lightDir), 0.0);
}

float calcSpecFac(vec3 normal, vec3 halfwayDir, float shininess)
{
	return pow(max(dot(normal, halfwayDir), 0.0), shininess);
}

vec3 calcLight(Material mat, 
				vec3 lightAmb, vec3 lightDiff, vec3 lightSpec, 
				vec3 lightDir, vec3 texelColor, vec3 normal, vec3 viewDir,
				float shadowFac)
{
	//Ambient
	vec3 ambient = calcAmbient(lightAmb, mat.ambient);

	// diffuse shading
    float diff = calcDiffFac(normal, lightDir);
    vec3 diffuse = calcDiffuse(lightDiff, mat.diffuse , diff);

	//Specular
	vec3 halfwayDir = getHalfwayDirVec(lightDir, viewDir);
	float spec = calcSpecFac(normal, halfwayDir, mat.shininess);
	vec3 specular = calcSpecular(lightSpec, mat.specular, spec);
	

	vec3 result = (ambient + (1. - shadowFac) * (diffuse + specular)) * texelColor;
	
	return result;
}


vec3 CalcDirLight(DirectionalLight light, Material mat, vec3 texelColor, vec3 normal, vec3 viewDir, float shadowFac)
{
	vec3 lightDir = normalize(-light.direction.xyz);
    
	return calcLight(mat, 
				light.ambient.xyz, light.diffuse.xyz, light.specular.xyz, 
				lightDir, texelColor, normal, viewDir, shadowFac);
}

float getAttenuation(PointLight light, vec3 fragPos)
{
	float distance = length(light.position.xyz - fragPos);
	float attenuation = 1.0 / (1. + light.linear * distance + 
    		    light.quadratic * (distance * distance));
				
	return attenuation;
}


vec3 CalcPointLight(PointLight light, Material mat, vec3 texelColor, vec3 normal, 
					vec3 fragPos, vec3 viewDir, float shadowFac)
{
	vec3 lightDir = normalize(light.position.xyz - fragPos);
	
	vec3 result = calcLight(mat, 
							light.ambient.xyz, light.diffuse.xyz, light.specular.xyz, 
							lightDir, texelColor, normal, viewDir, shadowFac);
	
	//Perform attenuation
	float attenuation = getAttenuation(light, fragPos); 

	return result * attenuation;
} 
#endif