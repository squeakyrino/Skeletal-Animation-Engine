#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 2) in vec2 texCoords; 

out vec2 oTexCoords;

void main()
{	
    oTexCoords = texCoords;

	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}