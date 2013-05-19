/*
 * Logger.hpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <vector>
#include <string>
#include <fstream>

#include <time.h>

#include <iostream>

namespace Fumasu
{

class Logger
{
public:
	Logger();
	~Logger();

	static Logger* getInstance();

	void print(std::string s);
	void flush();
private:
	static Logger* instance;

	std::vector<std::string> content;
	std::ofstream Datei;
};

} /* namespace Fumasu */
#endif /* LOGGER_HPP_ */
