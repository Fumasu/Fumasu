/*
 * Mouse.hpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include <iostream>

namespace Fumasu
{

class Mouse
{
public:
	Mouse();

	static Mouse* getInstance();

	void getPosition(int &x, int &y);
	bool isPressed(int b);
	bool wasPressed(int b);

	void setButton(int b, bool flag =true);
	void setPosition(int x, int y);
private:
	static Mouse *instance;

	int x, y;
	bool button[6];
	bool was[6];
};

} /* namespace Fumasu */
#endif /* MOUSE_HPP_ */
