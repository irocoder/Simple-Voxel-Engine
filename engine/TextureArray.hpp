#ifndef TEXTURE_ARRAY_HPP
#define TEXTURE_ARRAY_HPP

#include "Texture2D.hpp"
#include <vector>

class TextureArray : public Texture2D
{
	public:
		TextureArray(const std::string& file, int textureWidth, int textureHeight);
		void bind();
		void unbind();

		GLuint loadPNG(const std::string& file, int elementWidth, int elementHeight);

	protected:
		void upload(char* data, int elementWidth, int elementHeight);
};

#endif