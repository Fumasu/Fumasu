/*
 * GLTexture.h
 *
 *  Created on: 18.12.2011
 *      Author: Fumasu
 */

#ifndef GLTEXTURE_H_
#define GLTEXTURE_H_

#include <windows.h>

#include <string>
#include <GL/glew.h>

#include <cstring>

#include <iostream>

#include "FreeImage.h"

namespace Fumasu
{

class GLTexture
{
public:
	GLTexture();
	GLTexture(int width, int height);
	GLTexture(std::string filename);
	~GLTexture();

	void Filter();
	bool Load(std::string filename);
	void Bind();
	void UnBind();

	void Update();
	void SetChromaColor(unsigned int color);

	int GetWidth()
	{
		return width;
	}
	int GetHeight()
	{
		return height;
	}

	bool isOK()
	{
		return ok;
	}

	bool hasAlpha()
	{
		return alpha;
	}

	void saveToTGA(const std::string filename);
private:
	int width, height;
	GLuint pointer;
	bool ok, alpha;
};

} /* namespace Fumasu */
#endif /* GLTEXTURE_H_ */
