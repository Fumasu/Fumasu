#ifndef TIMER_2_H
#define TIMER_2_H

#include "SDL/SDL.h"

namespace Fumasu
{

class Timer_2
{
public:
	/** Default constructor */
	Timer_2();

	void start();
	void stop();
	unsigned int get_ticks();
	bool is_running();
protected:
private:
	unsigned int ticks;
	bool running;
};

}

#endif // TIMER_H
