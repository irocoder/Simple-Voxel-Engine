#include "CubeMap.hpp"
#include "Util.hpp"
#include <iostream>
#include <cassert>

CubeMap::CubeMap()
{
}

CubeMap::CubeMap(const std::string& fileName)
{
	loadPNG(fileName);	
}

void CubeMap::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setTop(int x, int y, int width, int height)
{
	top[0] = x;
	top[1] = y;
	top[2] = width;
	top[3] = height;
}

void CubeMap::setBottom(int x, int y, int width, int height)
{
	bottom[0] = x;
	bottom[1] = y;
	bottom[2] = width;
	bottom[3] = height;
}

void CubeMap::setRight(int x, int y, int width, int height)
{
	right[0] = x;
	right[1] = y;
	right[2] = width;
	right[3] = height;
}

void CubeMap::setLeft(int x, int y, int width, int height)
{
	left[0] = x;
	left[1] = y;
	left[2] = width;
	left[3] = height;
}

void CubeMap::setFront(int x, int y, int width, int height)
{
	front[0] = x;
	front[1] = y;
	front[2] = width;
	front[3] = height;
}

void CubeMap::setBack(int x, int y, int width, int height)
{
	back[0] = x;
	back[1] = y;
	back[2] = width;
	back[3] = height;
}


void CubeMap::loadPNG(const std::string& filePNG)
{
	//std::string fileName; //just in case
	//fileName = filePNG;

	//FILE* file = fopen(fileName.c_str(), "rb");
	//assert(file);

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

		return;
	}

	png_set_read_fn(pngPtr, &file, readPNGData);

	//read the PNG file
	png_set_sig_bytes(pngPtr, PNG_SIG_SIZE);
	png_read_info(pngPtr, infoPtr);

	int width = png_get_image_width(pngPtr, infoPtr); //add width/height as private variables?
	int height = png_get_image_height(pngPtr, infoPtr);

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

	std::cout << "PNG LOADED" << std::endl;

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	/*glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);*/

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right[2], right[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left[2], left[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top[2], top[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottom[2], bottom[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front[2], front[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back[2], back[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, right[0], right[1], right[2], right[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, left[0], left[1], left[2], left[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, top[0], top[1], top[2], top[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, bottom[0], bottom[1], bottom[2], bottom[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, front[0], front[1], front[2], front[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, back[0], back[1], back[2], back[3], GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)newData);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	delete[] rowPtrs;
	delete[] data;
	delete[] newData;
}