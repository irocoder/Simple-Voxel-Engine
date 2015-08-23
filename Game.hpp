#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include <glfw3.h>
#include <chrono>
#include <memory>
#include "engine/ShaderProgram.hpp"
#include "engine/Camera.hpp"
#include "engine/VoxelBatch.hpp"
#include "Mob.hpp"
#include "engine/Texture2D.hpp"
#include "engine/CubeMap.hpp"
#include "Map.hpp"

class Game
{
	public:
		Game(const std::string& title, int width, int height);

		void start();
		void getInput();
		void update();
		void draw();

	private:
		
		GLFWwindow* window;
		std::unique_ptr<Map> map;
		std::unique_ptr<ShaderProgram> shaderProgram;
		std::unique_ptr<Camera> camera;
		std::unique_ptr<VoxelBatch> primitiveBatch;
		std::unique_ptr<CubeMap> cubeMap;
		std::unique_ptr<Texture2D> texture;

		std::unique_ptr<RectangularPrism> prism;
		std::unique_ptr<Mob> mob;
		std::unique_ptr<Mob> mob1;

		std::vector<std::unique_ptr<Voxel>> blocks;

		float angle = 0;

		float width, height;
		std::string title;
		
		std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> loopBegin, loopEnd, nextLoop;
		const int MAX_FPS = 60;
		double fps;
		const int NSPS = 1000000000;
		const std::chrono::nanoseconds TIME_BETWEEN_TICKS = std::chrono::nanoseconds(NSPS / MAX_FPS);

};

#endif