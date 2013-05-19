/*
 * VertexArray.cpp
 *
 *  Created on: 24.12.2011
 *      Author: Fumasu
 */

#include "Fumasu/VertexArray.h"

namespace Fumasu {

VertexArray::~VertexArray()
{
}

void VertexArray::AddTexCoord(double x, double y)
{
	tex_data.push_back(x);
	tex_data.push_back(y);
}

void VertexArray::AddVertexCoord(double x, double y, double z)
{
	vertex_data.push_back(x);
	vertex_data.push_back(y);
	vertex_data.push_back(z);
}

void VertexArray::AddNormalCoord(double x, double y, double z)
{
	normal_data.push_back(x);
	normal_data.push_back(y);
	normal_data.push_back(z);
}

void VertexArray::Activate()
{
	glVertexPointer(3, GL_DOUBLE, 0, &vertex_data.front());
	glTexCoordPointer(2, GL_DOUBLE, 0, &tex_data.front());
	glNormalPointer(3, 0, &normal_data.front());
	glColorPointer(4, GL_DOUBLE, 0, &color_data.front());
}

void VertexArray::clear()
{
	vertex_data.clear();
	normal_data.clear();
	tex_data.clear();
	color_data.clear();
}

void VertexArray::AddColorValue(double r, double g, double b, double a)
{
	color_data.push_back(r);
	color_data.push_back(g);
	color_data.push_back(b);
	color_data.push_back(a);
}

} //namespace
