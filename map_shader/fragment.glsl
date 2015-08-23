#version 330

uniform samplerCube cube;
uniform sampler2D texture;
uniform sampler2DArray textureArray;

in vec4 vertColor;
out vec4 outColor;

in vec3 texCoordOut;

void main()
{
	//outColor = texture(texture, texCoordOut) * vertColor;
	if(texCoordOut.z < 0)
		outColor = vertColor;
	else
		outColor = texture(textureArray, texCoordOut) * vertColor;

	//outColor = vertColor;
}