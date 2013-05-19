/*
 * Sprite.cpp
 *
 *  Created on: 27.02.2012
 *      Author: Fumasu
 */

#include "Fumasu/Sprite.hpp"

namespace Fumasu
{

Sprite::Sprite(std::string filename, int tiles_x, int tiles_y)
{
	this->tiles_x =tiles_x;
	this->tiles_y =tiles_y;
	texture.Load(filename);

	tile_width =texture.GetWidth() / tiles_x;
	tile_height =texture.GetHeight() / tiles_y;

}

void Sprite::Bind()
{
	texture.Bind();
}

void Sprite::GetTexCoord(int x, int y, double &u, double &v)
{
	u =(x * tile_width * 1.0) / texture.GetWidth();
	v =(1 - (double)tile_height / texture.GetHeight()) - (y * tile_height * 1.0) / texture.GetHeight();
}

void Sprite::GetTileSize(double &w, double &h)
{
	w =(double)tile_width / (double)texture.GetWidth();
	h =(double)tile_height / (double)texture.GetHeight();
}

void Sprite::GetSize(double &w, double &h)
{
	w =texture.GetWidth() / tiles_x;
	h =texture.GetHeight() / tiles_y;
}

}

/* namespace Fumasu */
