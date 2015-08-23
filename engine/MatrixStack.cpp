#include "MatrixStack.hpp"
#include "Constants.hpp"
#include <glm/gtc/quaternion.hpp>


void MatrixStack::rotateX(float angle)
{
	curMatrix *= createRotationX(angle);
}

void MatrixStack::rotateY(float angle)
{
	curMatrix *= createRotationY(angle);
}

void MatrixStack::rotateZ(float angle)
{
	curMatrix *= createRotationZ(angle);
}

void MatrixStack::rotate(float pitch, float yaw, float roll)
{
	curMatrix *= createRotation(pitch, yaw, roll);
}

void MatrixStack::scale(float x, float y, float z)
{
	curMatrix *= createScale(x, y, z);
}

void MatrixStack::translate(float x, float y, float z)
{
	curMatrix *= createTranslation(x, y, z);
}

void MatrixStack::push()
{
	matrices.push(curMatrix);
}

void MatrixStack::pop()
{
	matrices.pop();
	curMatrix = matrices.top();
}

glm::mat4 MatrixStack::createRotationX(float angle)
{
	glm::mat4 matrix;
	float r = angle * PI / 180; //radians
	float cos = std::cos(r);
	float sin = std::sin(r);

	matrix[1][1] = cos;
	matrix[2][1] = -sin;
	matrix[1][2] = sin;
	matrix[2][2] = cos;

	return matrix;
}

glm::mat4 MatrixStack::createRotationY(float angle)
{
	glm::mat4 matrix;
	float r = angle * PI / 180; //radians
	float cos = std::cos(r);
	float sin = std::sin(r);

	matrix[0][0] = cos;
	matrix[2][0] = sin;
	matrix[0][2] = -sin;
	matrix[2][2] = cos;

	return matrix;
}

glm::mat4 MatrixStack::createRotationZ(float angle)
{
	glm::mat4 matrix;
	float r = angle * PI / 180; //radians
	float cos = std::cos(r);
	float sin = std::sin(r);

	matrix[0][0] = cos;
	matrix[1][0] = -sin;
	matrix[0][1] = sin;
	matrix[1][1] = cos;

	return matrix;
}

glm::mat4 MatrixStack::createRotation(float pitch, float yaw, float roll)
{
	float p = glm::radians(pitch);
	float y = glm::radians(yaw);
	float r = glm::radians(roll);

	glm::quat rotation(glm::vec3(p, y, r));
	return glm::mat4_cast(rotation);
}

glm::mat4 MatrixStack::createScale(float x, float y, float z)
{
	glm::mat4 matrix;
	
	matrix[0][0] = x;
	matrix[1][1] = y;
	matrix[2][2] = z;

	return matrix;
}

glm::mat4 MatrixStack::createTranslation(float x, float y, float z)
{
	glm::mat4 matrix;

	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;

	return matrix;
}

void MatrixStack::clear()
{
	while(!matrices.empty())
	{
		matrices.pop();
	}

	curMatrix = glm::mat4();
}