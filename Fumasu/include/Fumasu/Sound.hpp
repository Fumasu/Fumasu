/*
 * Sound.hpp
 *
 *  Created on: 29.02.2012
 *      Author: Fumasu
 */

#ifndef SOUND_HPP_
#define SOUND_HPP_

#include <SDL/SDL_mixer.h>

#include <string>
#include <iostream>

namespace Fumasu
{

class Sound
{
public:
	Sound(std::string filename);

	void Play();
private:
	Mix_Chunk *sample;
};

} /* namespace Fumasu */
#endif /* SOUND_HPP_ */
