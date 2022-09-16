#version 330 core

out vec4 FragColor;
  
in vec2 oTexCoords;

uniform sampler2D screenTexture;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, oTexCoords).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);

    //FragColor = vec4(texture(screenTexture, oTexCoords).rgb, 1);
}  