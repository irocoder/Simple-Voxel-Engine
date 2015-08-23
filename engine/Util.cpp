#include "Util.hpp"

#include <istream>

void readPNGData(png_structp pngPtr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(pngPtr);

	((std::istream*)a)->read((char*)data, length);
}
