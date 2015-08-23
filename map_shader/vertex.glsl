#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 texCoord;

smooth out vec4 vertColor;
out vec3 texCoordOut;

uniform mat4 combined;

void main()
{
	gl_Position = combined * position;
	vertColor = color;
	texCoordOut = texCoord;
}