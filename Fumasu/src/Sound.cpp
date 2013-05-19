/*
 * Sound.cpp
 *
 *  Created on: 29.02.2012
 *      Author: Fumasu
 */

#include "Fumasu/Sound.hpp"

namespace Fumasu
{

Sound::Sound(std::string filename)
{
	sample =Mix_LoadWAV(filename.c_str());
}

void Sound::Play()
{
	Mix_PlayChannel(0, sample, 0);
}

} /* namespace Fumasu */
