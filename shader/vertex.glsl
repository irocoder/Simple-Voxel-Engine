#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 translate;

smooth out vec4 vertColor;

uniform mat4 combined;
uniform mat4 lookAt;
uniform mat4 perspective;
uniform mat4 transform;

void main()
{
	//vec4 temp = transform * (position);
	vec4 temp = vec4(translate.x, translate.y, translate.z, 0.0f) + position;

	gl_Position = combined * temp;
	vertColor = color;
}