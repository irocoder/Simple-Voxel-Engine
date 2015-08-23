#ifndef CUBE_MAP_HPP
#define CUBE_MAP_HPP

#include "Texture2D.hpp"
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <glm/glm.hpp>

class CubeMap
{
	public:
		CubeMap();
		CubeMap(const std::string& fileName);
		
		void bind();
		void unbind();

		void setTop(int x, int y, int width, int height);
		void setBottom(int x, int y, int width, int height);
		void setRight(int x, int y, int width, int height);
		void setLeft(int x, int y, int width, int height);
		void setFront(int x, int y, int width, int height);
		void setBack(int x, int y, int width, int height);

		void loadPNG(const std::string& fileName);
		void deleteTexture();

	private:
		GLuint id; //texture ID
		std::ifstream file;
		glm::vec4 top, bottom, right, left, front, back;
};



#endif