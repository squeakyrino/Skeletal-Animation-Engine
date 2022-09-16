#version 330 core

out vec4 FragColor;
  
in vec2 oTexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 700.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    float edgeKernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    float blurKernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, oTexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * blurKernel[i];
    
    FragColor = vec4(col, 1.0);

    //Uncomment for kernel madness
    FragColor = texture(screenTexture, oTexCoords);



    /*Depth show* /
    float depthValue = texture(screenTexture, oTexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
    /**/
}  