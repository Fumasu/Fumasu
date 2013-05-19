/*
 * Keys.hpp
 *
 *  Created on: 05.03.2012
 *      Author: Fumasu
 */

#ifndef KEYS_HPP_
#define KEYS_HPP_

#include <iostream>

namespace Fumasu
{

class Keys
{
public:
	Keys();

	bool isPressed(int c);
	bool wasPressed(int c);

	void setKey(int c, bool flag =true);
	void update(bool is[323]);

	static Keys* getInstance();
private:
	static Keys *instance;
	bool is[323];
	bool was[323];
};

} /* namespace Fumasu */
#endif /* KEYS_HPP_ */
