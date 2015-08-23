#include "TextureArray.hpp"
#include <png.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include "Util.hpp"

TextureArray::TextureArray(const std::string& file, int textureWidth, int textureHeight)
{
	//replace hard-coded values
	loadPNG(file, textureWidth, textureHeight);	
}

void TextureArray::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

void TextureArray::unbind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, NULL);
}

void TextureArray::upload(char* data, int elementWidth, int elementHeight)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);

	std::cout << "NUMBER OF TILES: " << (width / elementWidth) * (height / elementHeight) << std::endl;

	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, elementWidth, elementHeight, (width / elementWidth) * (height / elementHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, elementWidth, elementHeight, (width / elementWidth) * (height / elementHeight));

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, elementWidth, elementHeight, (width / elementWidth) * (height / elementHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, elementWidth, elementHeight, (width / elementWidth) * (height / elementHeight), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D_ARRAY, NULL);
	/*for(int y = 0; y < (height / elementHeight); y++)
	{
		for(int x = 0; x < (width / elementWidth); x++)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, x * elementWidth, y * elementHeight, y * (height / elementHeight) + x, elementWidth, elementHeight,
		}
	}*/
}

GLuint TextureArray::loadPNG(const std::string& filePNG, int elementWidth, int elementHeight)
{
	file.open(filePNG, std::ios::binary);

	unsigned char header[8];
	file.read((char*)header, PNG_SIG_SIZE);
	//fread(header, 1, PNG_SIG_SIZE, file);

	if (!file.good())
	{
		std::cout << "FILE NOT GOOD!" << std::endl;
	}

	if (png_sig_cmp(header, 0, 8))
	{
		std::cout << filePNG.c_str() << " is not a PNG file" << std::endl;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	assert(pngPtr);

	png_infop infoPtr = png_create_info_struct(pngPtr);
	assert(infoPtr);

	png_bytep* rowPtrs = NULL;
	char* data = NULL;

	if (setjmp(png_jmpbuf(pngPtr)))
	{
		//this block executes when a libPNG error has occured
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);

		delete[] rowPtrs;
		delete[] data;

		std::cout << "An error occured reading the PNG file." << std::endl;

		return 0;
	}

	png_set_read_fn(pngPtr, &file, readPNGData);
	/*png_init_io(pngPtr, &file);*/

	//read the PNG file
	png_set_sig_bytes(pngPtr, PNG_SIG_SIZE);
	png_read_info(pngPtr, infoPtr);

	width = png_get_image_width(pngPtr, infoPtr);
	height = png_get_image_height(pngPtr, infoPtr);

	//bit depth per channel
	png_uint_32 bitDepth = png_get_bit_depth(pngPtr, infoPtr);

	//number of channels
	png_uint_32 channels = png_get_channels(pngPtr, infoPtr);

	//color type (RGB, RGBA)
	png_uint_32 colorType = png_get_color_type(pngPtr, infoPtr);

	switch (colorType)
	{
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(pngPtr);
		channels = 3;
		break;

	case PNG_COLOR_TYPE_GRAY:
		if (bitDepth < 8)
		{
			png_set_expand_gray_1_2_4_to_8(pngPtr);
			bitDepth = 8;
		}
		break;
	}

	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		channels++;
	}

	//strip the bit depth down to 8 if it is 16 //WHY?!  CHECK THIS!
	if (bitDepth == 16)
	{
		png_set_strip_16(pngPtr);
	}

	//create an array of row pointers, one for every row, on the heap
	rowPtrs = new png_bytep[height];

	//allocate a buffer with enough space (in bytes)
	int dataSize = width * height * bitDepth * channels / 8;
	data = new char[dataSize];
	int dataWidth = dataSize / height; //length of a row in the data array

	//length in bytes of one row //MIGHT BE CULPRIT OF NON-WORKING 32-BIT PNG LOADING
	const unsigned int stride = width * bitDepth * channels / 8;

	for (size_t i = 0; i < height; i++)
	{
		png_uint_32 q = (height - i - 1) * stride;
		rowPtrs[i] = (png_bytep)data + q;
	}

	png_read_image(pngPtr, rowPtrs); //application crashes here

	//fclose(file);

	//flip the image
	char* newData = new char[dataSize];

	for (int i = dataSize - dataWidth; i + 1 > 0; i -= dataWidth)
	{
		for (int k = 0; k < dataWidth; k++)
		{
			newData[dataSize - i - dataWidth + k] = data[i + k];
		}
	}

	upload(newData, elementWidth, elementHeight);

	png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);

	delete[] rowPtrs;
	delete[] data;
	delete[] newData;

	std::cout << "PNG LOADED!" << std::endl;

	return id;
}