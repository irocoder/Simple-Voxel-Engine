#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "engine/ShaderProgram.hpp"
#include <glfw3.h>
#include <iostream>
#include <memory>
#include "engine/VoxelBatch.hpp"
#include "Mob.hpp"
#include "engine/Constants.hpp"
#include "engine/Camera.hpp"
#include "Game.hpp"

int main()
{
	if(!glfwInit())
		std::cout << "GLFW INIT FAILURE!" << std::endl;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	Game game("XarGame", 800, 600);
	game.start();

	std::cout << "END!" << std::endl;
}