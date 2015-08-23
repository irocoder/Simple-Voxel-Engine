#include "Game.hpp"
#include <thread>
#include <iostream>
#include "engine/Texture2D.hpp"

Game::Game(const std::string& title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = true;
	glewInit();

	glViewport(0, 0, width, height);


	map = std::make_unique<Map>(100, 25, 100);
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 5.0f), window, map->getShaderProgram(), width, height, 0.01f, 5000.0f);

	//mob = std::make_unique<Mob>(
	shaderProgram = std::make_unique<ShaderProgram>("../shader/vertex.glsl", "../shader/fragment.glsl");
	prism = std::make_unique<RectangularPrism>(0, 0, 0, 3, 4, 5, 1.f, 0.f, 0.f, 1.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_DEPTH_CLAMP);
}	

void Game::update()
{	
}

void Game::draw()
{	
	map->draw(camera.get());

	shaderProgram->bind();
	camera->update();
	//mob->draw();
	shaderProgram->unbind();
}

void Game::getInput()
{
}

void Game::start()
{
	while(!glfwWindowShouldClose(window))
	{
		if(std::chrono::high_resolution_clock::now() >= nextLoop) //be sure to set nextupdate in ctor
		{
			//std::cout << "FPS: " << fps << "\n";

			loopBegin = std::chrono::high_resolution_clock::now();
			nextLoop = loopBegin + TIME_BETWEEN_TICKS;

			getInput();
			glfwPollEvents();

			update();
			draw();

			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> b = std::chrono::high_resolution_clock::now();
			glfwSwapBuffers(window);
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> e = std::chrono::high_resolution_clock::now();


			//std::cout << "DURATION: " << (e - b).count() << "\n";

			loopEnd = std::chrono::high_resolution_clock::now();

			if(loopEnd - loopBegin > TIME_BETWEEN_TICKS)
			{
				fps = ((double)TIME_BETWEEN_TICKS.count() / (double)(loopEnd - loopBegin).count());
			}
			else
			{
				fps = MAX_FPS;
			}
		}
		else
		{
			std::this_thread::sleep_for(nextLoop - std::chrono::high_resolution_clock::now());
		}
	}
	
	camera.reset();
	map.reset();
	shaderProgram.reset();
	primitiveBatch.reset();
	cubeMap.reset();
	texture.reset();
	mob.reset();
	mob1.reset();


	glfwDestroyWindow(window);
	glfwTerminate();
}