#include "BMPLoader.h"

void BMPLoader::load_BMP_to_image(const char* filename, Image& image)
{
	unsigned int* data_buffer[2] = { nullptr, nullptr };	// Header Buffers
	unsigned char* pixel_array = nullptr;	// Pixel Array (that holds the image)

	BITMAPFILEHEADER* bmp_file_header = nullptr;	// File Header for the BMP
	BITMAPINFOHEADER* bmp_info_header = nullptr;	// Info Header for the BMP

	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		std::cout << filename << " is not a valid file location.\n";
		return;
	}

	data_buffer[0] = new unsigned int[sizeof(BITMAPFILEHEADER)];
	data_buffer[1] = new unsigned int[sizeof(BITMAPINFOHEADER)];

	file.read((char*)data_buffer[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)data_buffer[1], sizeof(BITMAPINFOHEADER));

	bmp_file_header = (BITMAPFILEHEADER*) data_buffer[0];
	bmp_info_header = (BITMAPINFOHEADER*) data_buffer[1];

	// If the file isn't a .bmp file, we want to exit
	if (bmp_file_header->bfType != 0x4D42)
	{
		std::cout << "The File at " << filename << " is not a .bmp file!\n";
		return;
	}

	image.width = bmp_info_header->biWidth;
	image.height = bmp_info_header->biHeight;

	pixel_array = new unsigned char[image.width * image.height * 3];

	file.seekg(bmp_file_header->bfOffBits);
	file.read((char*)pixel_array, bmp_info_header->biSizeImage);	

	std::cout << "Width: " << image.width << '\n';
	std::cout << "Height: " << image.height << '\n';

	image.pixel_array = new unsigned int[image.width * image.height];

	// Colors are stored as BGR, and we want 0xRRGGBB. Hence, >> 16 for R, >> 8 for G, and >> 0 for B
	for (int i = 0; i < image.width * image.height * 3; i += 3)
	{
		unsigned int temp_color = (pixel_array[i]) + (pixel_array[i + 1] << 8) + (pixel_array[i + 2] << 16);
		image.pixel_array[(i / 3)] = temp_color;
	}

	std::cout << "Finished Loading " << filename << "!\n";

	delete data_buffer[0];
	delete data_buffer[1];
	delete[] pixel_array;

	file.close();	
}

