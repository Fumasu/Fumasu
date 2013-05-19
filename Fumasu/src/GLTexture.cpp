/*
 * GLTexture.cpp
 *
 *  Created on: 18.12.2011
 *      Author: Fumasu
 */

#include "Fumasu/GLTexture.hpp"

#include <fstream>
#include <vector>
#include <cstring>

namespace Fumasu
{

GLTexture::GLTexture()
{
	width = height = pointer = 0;
	ok = false;
	alpha =false;
}

GLTexture::GLTexture(int width, int height)
{
	std::string data(width * height * 4, '\0');

	glGenTextures(1, &pointer);
	glBindTexture(GL_TEXTURE_2D, pointer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.c_str());

	this->width =width;
	this->height =height;
	ok =true;
	alpha =false;
}



GLTexture::GLTexture(std::string filename)
{
	//GLTexture();
	Load(filename);
}

GLTexture::~GLTexture()
{
	ok = false;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &pointer);
}

void GLTexture::Filter()
{
}

void GLTexture::saveToTGA(const std::string filename) {
	struct TGAHEADER {
		unsigned char ID_length;
		unsigned char colorMapType;
		unsigned char imageType;
		unsigned char colorMap[5];
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		unsigned char bpp;
		unsigned char descriptor;
	};

	TGAHEADER header;
	std::vector<unsigned char> buffer(this->width * this->height * 4);

	memset(&header, 0, sizeof(TGAHEADER));
	header.imageType =2;
	header.width =this->width;
	header.height =this->height;
	header.bpp =32;

	this->Bind();

	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, &buffer[0]);

	std::fstream file(filename.c_str(), std::fstream::binary | std::fstream::trunc | std::fstream::out);

	file.write((char*)&header, sizeof(TGAHEADER));
	for(unsigned int i =0;i <buffer.size();i++) {
		file.put(buffer[i]);
	}
	//file.write((char*)&buffer, this->width * this->height * 4);

	file.close();
}

bool GLTexture::Load(std::string filename)
{

	if(pointer !=0)
	{
		glDeleteTextures(1, &pointer);
		pointer =0;
		width =0;
		height =0;
		ok =false;
		alpha =false;
	}

	FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
	FIBITMAP *image(0);
	BYTE *data(0);
	unsigned int width(0), height(0);
	int img_format = GL_RGB;
	int internal_format =GL_RGBA;

	format = FreeImage_GetFileType(filename.c_str(), 0);
	if (format == FIF_UNKNOWN) format = FreeImage_GetFIFFromFilename(filename.c_str());

	if (format == FIF_UNKNOWN)
	{
		ok = false;
		return false;
	}

	if (FreeImage_FIFSupportsReading(format)) image = FreeImage_Load(format, filename.c_str());

	if (!image)
	{
		ok = false;
		return false;
	}

	unsigned bpp = FreeImage_GetBPP(image);
	if (bpp < 24)
	{
		image = FreeImage_ConvertTo24Bits(image);
		bpp = FreeImage_GetBPP(image);
	}

	data = FreeImage_GetBits(image);
	width = FreeImage_GetWidth(image);
	height = FreeImage_GetHeight(image);

	if (width == 0 || height == 0 || data == 0)
	{
		ok = false;
		return false;
	}

	/*if (bpp >= 32 && FI_RGBA_RED < FI_RGBA_BLUE) img_format = GL_RGBA;
	else if(bpp >=32) img_format =GL_BGRA;
	else if(FI_RGBA_RED < FI_RGBA_BLUE) img_format = GL_RGB;
	else img_format =GL_BGR;*/

	if(FreeImage_GetColorType(image) ==FIC_RGB)
	{
		if(FI_RGBA_RED < FI_RGBA_BLUE)
		{
			img_format =GL_RGB;
		}
		else
		{
			img_format =GL_BGR;
		}

		internal_format =GL_RGB;
		alpha =false;
	}
	if(FreeImage_GetColorType(image) ==FIC_RGBALPHA)
	{
		if(FI_RGBA_RED < FI_RGBA_BLUE)
		{
			img_format =GL_RGBA;
		}
		else
		{
			img_format =GL_BGRA;
		}

		internal_format =GL_RGBA;
		alpha =true;
	}

	glGenTextures(1, &pointer);
	glBindTexture(GL_TEXTURE_2D, pointer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, img_format, GL_UNSIGNED_BYTE, data);

	this->width = width;
	this->height = height;

	ok = true;

	return true;
}

void GLTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, pointer);
}

void GLTexture::Update()
{
}

void GLTexture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::SetChromaColor(unsigned int color)
{
	int r, g, b;

	r = color & 0xff0000;
	r = r >> 16;
	g = color & 0x00ff00;
	g = g >> 8;
	b = color & 0x0000ff;

	GLubyte *data;
	GLint width, height;

	Bind();

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	data = new GLubyte[width * height * 4];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int pixel_address = (x + y * width) * 4;

			if (data[pixel_address] == r && data[pixel_address + 1] == g && data[pixel_address + 2] == b)
			{
				data[pixel_address + 0] = 0;
				data[pixel_address + 1] = 0;
				data[pixel_address + 2] = 0;
				data[pixel_address + 3] = 0;
			}
		}
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	alpha =true;
}

/* namespace Fumasu */
}
