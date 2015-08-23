#include "Camera.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Constants.hpp"

Camera::Camera(glm::vec3& position, GLFWwindow* window, GLint shaderProgram, int width, int height, float zNear, float zFar)
{
	program = shaderProgram;
	this->window = window;

	moveSpeed = 3.75f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;

	perspective = glm::perspective(45.f, (float)width / (float)height, zNear, zFar);
	//perspective = createPerspective(1.f, zNear, zFar, (float)width / (float)height);
	lookAt = glm::lookAt(position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	this->position = position;

	combinedUnif = glGetUniformLocation(program, "combined");
	perspectiveUnif = glGetUniformLocation(program, "perspective");
	lookAtUnif = glGetUniformLocation(program, "lookAt"); 
}

void Camera::rotate(float pitch, float yaw, float roll)
{
	quatRotation = glm::quat(glm::vec3(pitch, yaw, roll));
	rotation = glm::mat4_cast(quatRotation);
	
	//printMat(rotation);
}

void Camera::update()
{
	//check keyboard
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//camPos.z -= moveSpeed;
		position.x -= rotation[2][0] * moveSpeed;
		position.y -= rotation[2][1] * moveSpeed;
		position.z -= rotation[2][2] * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//camPos.z += moveSpeed;

		position.x += rotation[2][0] * moveSpeed;
		position.y += rotation[2][1] * moveSpeed;
		position.z += rotation[2][2] * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//camPos.x += moveSpeed;
		position.x += rotation[0][0] * moveSpeed;
		position.y += rotation[0][1] * moveSpeed;
		position.z += rotation[0][2] * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//camPos.x -= moveSpeed;
		position.x -= rotation[0][0] * moveSpeed;
		position.y -= rotation[0][1] * moveSpeed;
		position.z -= rotation[0][2] * moveSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		position.y += moveSpeed;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		position.y -= moveSpeed;
	}

	if(glfwGetKey(window, GLFW_KEY_UP))
	{
		pitch += 0.75f;
	}

	if(glfwGetKey(window, GLFW_KEY_DOWN))
	{
		pitch -= 0.75f;
	}

	if(glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		yaw -= 0.75f;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT))
	{
		yaw += 0.75f;
	}

	if(yaw > 360)
		yaw = 0;

	if(pitch > 360)
		pitch = 0;

	rotate(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
	lookAt = glm::lookAt(position, position - glm::vec3(rotation[2]), glm::vec3(rotation[1]));
	//TODO: MULTIPLY PERSPECTIVE & LOOKAT ON THE CPU!!!
	//glUniformMatrix4fv(perspectiveUnif, 1, GL_FALSE, glm::value_ptr(perspective));
	//glUniformMatrix4fv(lookAtUnif, 1, GL_FALSE, glm::value_ptr(lookAt));
	combined = perspective * lookAt;
	glUniformMatrix4fv(combinedUnif, 1, GL_FALSE, glm::value_ptr(combined));
}

glm::mat4 Camera::createPerspective(float frustumScale, float zNear, float zFar, float aspectRatio)
{
	glm::mat4 perspective;

	perspective[0][0] = frustumScale / (aspectRatio);
	perspective[1][1] = frustumScale;
	perspective[2][2] = (zFar + zNear) / (zNear - zFar);
	perspective[2][3] = -1.0f;
	perspective[3][2] = (2 * zFar * zNear) / (zNear - zFar);

	return perspective;
}