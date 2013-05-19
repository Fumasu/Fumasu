/*
 * Logger.cpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#include "Fumasu/Logger.hpp"

namespace Fumasu
{

Logger* Logger::instance =NULL;

Logger::Logger()
{
	Datei.open("log.txt");
	Datei <<"Start Logging" <<std::endl;
}

Logger::~Logger()
{
	Datei.close();
}

Logger* Logger::getInstance()
{
	if(instance ==NULL)
		instance =new Logger();

	return instance;
}

void Logger::print(std::string s)
{
	content.push_back(s);
	Datei <<s <<std::endl;
}

} /* namespace Fumasu */
