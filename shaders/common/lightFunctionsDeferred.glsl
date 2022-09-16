#ifndef LIGHTFUNC_H
#define LIGHTFUNC_H

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

vec3 calcLight(vec3 matAmb,vec3 matDiff, vec3 matSpec, float matShiny, 
				vec3 lightAmb, vec3 lightDiff, vec3 lightSpec, 
				vec3 lightDir, vec3 texelColor, vec3 normal, vec3 viewDir,
				float shadowFac)
{
	//Ambient
	vec3 ambient = calcAmbient(lightAmb, matAmb);

	// diffuse shading
    float diff = calcDiffFac(normal, lightDir);
    vec3 diffuse = calcDiffuse(lightDiff, matDiff, diff);

	//Specular
	vec3 halfwayDir = getHalfwayDirVec(lightDir, viewDir);
	float spec = calcSpecFac(normal, halfwayDir, matShiny);
	vec3 specular = calcSpecular(lightSpec, matSpec, spec);
	

	vec3 result = (ambient + (1. - shadowFac) * (diffuse + specular)) * texelColor;
	
	return result;
}

float PCFiltering(sampler2D depthMap, vec3 projCoords, float currentDepth, float bias)
{
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
	    for(int y = -1; y <= 1; ++y)
	    {
	        float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
	        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
	    }    
	}
	shadow /= 9.0;

	return shadow;
}

float ShadowCalculation(sampler2D depthMap, vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

	//Prevents shadows from outside the lightspace
     if(projCoords.z > 1.0)
        return 0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    //Bias to fix acne
	//Note: The normal passed here is actually the normal from the normal map
	//This causes weird results because of that.
	//Ask or something idk.

    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
	float bias = 0.005;
	//Get the shadow value using PCF
	float shadow = PCFiltering(depthMap, projCoords, currentDepth, bias);

    return shadow;
}  

/** /
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
/**/
#endif