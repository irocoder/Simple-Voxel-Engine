#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <png.h>

#define PNG_SIG_SIZE 8

class Texture2D
{
	public:
		Texture2D();
		Texture2D(const Texture2D& texture);
		Texture2D(const std::string& file);
		~Texture2D();

		void bind();
		void unbind();

		GLuint loadPNG(const std::string& filePNG);
		void deleteTexture();

		GLuint getID() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;

		int offsetX;
		int offsetY;

		Texture2D& Texture2D::operator=(const Texture2D texture);

	protected:
		void upload(char* data);
		GLuint id;
		unsigned int width;
		unsigned int height;

		std::string fileName;
		std::ifstream file;
};



#endif