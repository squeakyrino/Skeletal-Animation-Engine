#version 330 core

#include ../common/globalUniform.glsl //? #include "../common/globalUniform.glsl"
#include ../common/lightFunctionsDeferred.glsl //? #include "../common/lightFunctionsDeferred.glsl"

out vec4 FragColor;
 
in vec2 oTexCoords;

//Gbuffer stuff
uniform sampler2D posTex;
uniform sampler2D normalTex;
uniform sampler2D colorTex;
uniform sampler2D matAmbTex;
uniform sampler2D matDiffTex;
uniform sampler2D matSpecShinyTex;

//Light stuff
//TODO: No depth map for now because it's hard :)

uniform float radius;
uniform vec3 lightPos;
uniform vec3 lightAmb;
uniform vec3 lightDiff;
uniform vec3 lightSpec;

void main()
{   
    // retrieve data from G-buffer
    vec3 fragPos = texture(posTex, oTexCoords).rgb;
    vec3 normal = texture(normalTex, oTexCoords).rgb;
    vec3 uvTexColor = texture(colorTex, oTexCoords).rgb;
    vec3 matAmb = texture(matAmbTex, oTexCoords).rgb;
    vec3 matDiff = texture(matDiffTex, oTexCoords).rgb;
    vec3 matSpec = texture(matSpecShinyTex, oTexCoords).rgb;
    float shininess = texture(matSpecShinyTex, oTexCoords).a;

    /** /
    //TODO: No shadows for now
    //LightSpace frag pos
    vec4 lightSpaceFragPos = lightProjView * vec4(fragPos, 1.0);
    vec3 lightDirReverse = normalize(-lightDir);

    //Calculate shadow factor
    
    float shadowFac = ShadowCalculation(depthMap, lightSpaceFragPos, normal, lightDirReverse);
    /**/

    vec3 lightDirReverse = normalize(lightPos - fragPos);
    float shadowFac = 0.0f;

    vec3 viewDir = normalize(camPos.xyz - fragPos);

    vec3 outputLight = vec3(0);

    float distance = length((lightPos - fragPos));
    
    if(distance < radius)
    {
    //Calculate frag color (in linear space still)
    outputLight = calcLight(matAmb, matDiff, matSpec, shininess, 
				lightAmb, lightDiff, lightSpec, 
				lightDirReverse, uvTexColor, normal, viewDir,
				shadowFac);
    }
    

    FragColor = vec4(outputLight, 1.0);
}  