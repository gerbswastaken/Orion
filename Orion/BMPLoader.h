#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Image.h"


class BMPLoader
{
	public:
		static void load_BMP_to_image(const char* filename, Image& image);
		static void load_BMP_to_image_simple(const char* filename, Image& image);

	private:
		static const int header_length = 54;
		static const int header_width_position = 18;
		static const int header_height_position = 22;
};

#endif

