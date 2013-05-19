#include "Fumasu/Timer_2.hpp"

namespace Fumasu
{

Timer_2::Timer_2()
{
	ticks = 0;
	running = false;
}

void Timer_2::start()
{
	ticks = SDL_GetTicks();
	running = true;
}

void Timer_2::stop()
{
	ticks = 0;
	running = false;
}

unsigned int Timer_2::get_ticks()
{
	return SDL_GetTicks() - ticks;
}

bool Timer_2::is_running()
{
	return running;
}

}
