/*
 * Keys.cpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#include "Fumasu/Keys.hpp"

namespace Fumasu
{

Keys *Keys::instance =NULL;

Keys::Keys()
{
	for(int i =0;i <323;i++)
	{
		is[i] =false;
		was[i] =false;
	}
}

bool Keys::isPressed(int c)
{
	was[c] =is[c];

	return is[c];
}

bool Keys::wasPressed(int c)
{
	return was[c];
}

Keys* Keys::getInstance()
{
	if(instance ==NULL)
	{
		instance =new Keys();
	}

	return instance;
}

void Keys::setKey(int c, bool flag)
{
	if(!flag) was[c] =flag;
	is[c] =flag;
}

} /* namespace Fumasu */
