/*
 * SDLApp.h
 *
 *  Created on: 02.01.2012
 *      Author: Fumasu
 */

#ifndef SDLAPP_H_
#define SDLAPP_H_

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <string>

#include "Fumasu/Keys.hpp"
#include "Fumasu/Mouse.hpp"

using namespace std;

namespace Fumasu
{

#define VSYNC 1

class SDLApp
{
public:
	SDLApp(int width, int height, int bpp, string caption, bool Mode2D =true);
	~SDLApp();

	bool createWindow();
	bool destroyWindow();

	bool isRunning();
	bool isActive();
	void Run();

	void setOptions(char options);

	int getWidth();
	int getHeight();
	static void GetMousePos(int &x, int &y);

	void setOnResizeFunc(void(*func)(void));
	void setOnMouseMoveFunc(void(*func)(int, int));
private:
	static int width, height, bpp;
	static int mouse_x, mouse_y;
	string caption;
	bool done;
	bool active;
	bool Mode2D;
	SDL_Event Event;
	char flags;
	SDL_Surface *surface;
	Keys *keys;
	Mouse *mouse;

	void (*onResize)(void);
	void (*onMouseMove)(int x, int y);

	bool init_SDL();
	bool init_GL();
};

} /* namespace Fumasu */
#endif /* CSDLAPP_H_ */
