#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 transform;
uniform mat4 lightProjView;

void main()
{
    //Note: lightProj * lightView
    gl_Position = lightProjView * transform * vec4(pos, 1.0);
}  