/*
 * Mouse.cpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#include "Fumasu/Mouse.hpp"

namespace Fumasu
{

Mouse* Mouse::instance =NULL;

Mouse::Mouse()
{
	for(int i =0;i <6;i++)
	{
		was[i] =button[i] =false;
	}
}

Mouse* Mouse::getInstance()
{
	if(instance ==NULL) instance =new Mouse();

	return instance;
}

bool Mouse::isPressed(int b)
{
	//if(button[b]) was[b] =true;
	was[b] =button[b];

	return button[b];
}

bool Mouse::wasPressed(int b)
{
	return was[b];
}

void Mouse::setButton(int b, bool flag)
{
	if(flag ==false) was[b] =flag;
	button[b] =flag;
}

void Mouse::setPosition(int x, int y)
{
	this->x =x;
	this->y =y;
}

void Mouse::getPosition(int &x, int &y)
{
	x =this->x;
	y =this->y;
}

} /* namespace Fumasu */
