#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glfw3.h>

class Camera
{
	public:
		Camera(glm::vec3& position, GLFWwindow* window, GLint shaderProgram, int width, int height, float zNear, float zFar);

		void move(float speed, float x, float y, float z);
		void rotate(float pitch, float yaw, float roll);
		void update();

	private:
		GLFWwindow* window;
		int program;

		float pitch, yaw, roll;

		glm::vec3 position;
		glm::quat quatRotation;
		glm::mat4 rotation;

		glm::mat4 perspective;
		glm::mat4 lookAt;
		glm::mat4 combined;

		GLint perspectiveUnif;
		GLint lookAtUnif;
		GLint combinedUnif;

		float moveSpeed;

		glm::mat4 createPerspective(float frustumScale, float zNear, float zFar, float aspectRatio);
};



#endif