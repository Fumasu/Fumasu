/*
 * Timer.cpp
 *
 *  Created on: 04.04.2013
 *      Author: Fumasu
 */

#include "Fumasu/Timer.hpp"

namespace Fumasu
{

Timer::Timer() {
	_running =false;
}

void Timer::start() {
	_running =true;
	_time =Poco::Timestamp();
}

void Timer::stop() {
	_running =false;
}

unsigned int Timer::get_ticks() {
	Poco::Timestamp::TimeDiff diff;

	diff =Poco::Timestamp() - _time;
	return diff / 1000;
}

bool Timer::is_running() {
	return _running;
}

} /* namespace Fumasu */
