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
//Depth map
uniform sampler2D depthMap;

uniform vec3 lightDir;
//Light space transform
uniform mat4 lightProjView;
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

    //LightSpace frag pos
    vec4 lightSpaceFragPos = lightProjView * vec4(fragPos, 1.0);
    vec3 lightDirReverse = normalize(-lightDir);

    //Calculate shadow factor
    float shadowFac = ShadowCalculation(depthMap, lightSpaceFragPos, normal, lightDirReverse);

    vec3 viewDir = normalize(camPos.xyz - fragPos);

    //Calculate frag color (in linear space still)
    vec3 outputLight = calcLight(matAmb, matDiff, matSpec, shininess, 
				lightAmb, lightDiff, lightSpec, 
				lightDirReverse, uvTexColor, normal, viewDir,
				shadowFac);

    FragColor = vec4(outputLight, 1.0);
}  