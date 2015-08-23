#include "RectangularPrism.hpp"
#include "Constants.hpp"


const std::array<unsigned short, 36> RectangularPrism::indices =
	{ 0, 1, 2, 0, 2, 3, //front face
	3, 2, 6, 3, 6, 7, //bottom face
	1, 0, 4, 1, 4, 5, //top face
	5, 4, 7, 5, 7, 6, //back face
	2, 1, 5, 2, 5, 6, //right face
	0, 3, 7, 0, 7, 4  //left face
	};

RectangularPrism::RectangularPrism(float x, float y, float z, float width, float height, float depth, float r, float g, float b, float a)
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	pitch = 0.f;
	yaw = 0.f;
	roll = 0.f;

	/*m_translation[0][0] = 1;
	m_translation[1][1] = 1;
	m_translation[2][2] = 1;
	m_translation[3][3] = 1;*/

	setPosition(x, y, z);
}

void RectangularPrism::translate(float x, float y, float z)
{
	glm::mat4 translation;
	translation[3][0] = x;
	translation[3][1] = y;
	translation[3][2] = z;

	transform *= translation;
}

void RectangularPrism::scale(float x, float y, float z)
{
	
}

void RectangularPrism::rotate(float pitch, float yaw, float roll)
{
	float pitchRad = glm::radians(pitch);
	float yawRad = glm::radians(yaw);
	float rollRad = glm::radians(roll);

	glm::quat qRotation = glm::quat(glm::vec3(pitchRad, yawRad, rollRad));

	transform *= glm::mat4_cast(qRotation);
}		

void RectangularPrism::setColor(glm::vec4& color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

void RectangularPrism::setColor(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void RectangularPrism::setOffset(float x, float y, float z)
{
	this->offset[0] = x;
	this->offset[1] = y;
	this->offset[2] = z;
}

void RectangularPrism::setPosition(float x, float y, float z)
{
	transform[3][0] = x;
	transform[3][1] = y;
	transform[3][2] = z;
}

void RectangularPrism::setRotation(float x, float y, float z)
{
}

void RectangularPrism::setScale(float x, float y, float z)
{
}

void RectangularPrism::setTransform(glm::mat4& transform)
{
	this->transform = transform;
}

void RectangularPrism::update()
{
	
}