/*
 * Sprite.hpp
 *
 *  Created on: 27.02.2012
 *      Author: Fumasu
 */

#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include "Fumasu/GLTexture.hpp"
#include <string>
#include <vector>

#include <iostream>

namespace Fumasu
{

class Sprite
{
public:
	Sprite(std::string filename, int tiles_x, int tiles_y);

	void Bind();
	void GetTexCoord(int x, int y, double &u, double &v);
	void GetTileSize(double &w, double &h);
	void GetSize(double &w, double &h);
private:
	GLTexture texture;
	std::vector<GLTexture*> tiles;
	int tiles_x, tiles_y;
	int tile_width, tile_height;
};

} /* namespace Fumasu */
#endif /* SPRITE_HPP_ */
