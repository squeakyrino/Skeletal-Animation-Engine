#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec4 o_fragPosLightSpace;
} fs_in;

uniform sampler2D depthMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

     if(projCoords.z > 1.0)
        return 0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    //Bias to fix acne
    //TODO: Needs light dir
    float bias = 0.005;
    
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  


    return shadow;
}  

void main()
{	
    float shadowValue = ShadowCalculation(fs_in.o_fragPosLightSpace);

    gl_FragDepth = shadowValue;
    FragColor = vec4(vec3(shadowValue), 1.0);
}