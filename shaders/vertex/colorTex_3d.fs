#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 txCord;

out vec3 vertexColor;
out vec2 textureCord;

uniform mat4 model;
uniform mat4 view=mat4(1.0f);
uniform mat4 projection=mat4(1.0f);

void main()
{
	vertexColor=aColor;
	textureCord=txCord;
	gl_Position =projection*view*model*vec4(aPos, 1.0);
    //gl_Position=model*vec4(aPos,1.0);
}