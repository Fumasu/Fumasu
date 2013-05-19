/*
 * Timer.hpp
 *
 *  Created on: 04.04.2013
 *      Author: Fumasu
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <Poco/Timestamp.h>

namespace Fumasu
{

class Timer
{
public:
	Timer();

	void start();
	void stop();

	unsigned int get_ticks();
	bool is_running();
private:
	Poco::Timestamp _time;
	bool _running;
};

} /* namespace Fumasu */
#endif /* TIMER_HPP_ */
