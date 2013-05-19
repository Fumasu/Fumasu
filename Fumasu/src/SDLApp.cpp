/*
 * SDLApp.cpp
 *
 *  Created on: 02.01.2012
 *      Author: Fumasu
 */

#include "Fumasu/SDLApp.hpp"

#include <iostream>

namespace Fumasu
{

int SDLApp::mouse_x = 0;
int SDLApp::mouse_y = 0;
int SDLApp::width = 0;
int SDLApp::height = 0;
int SDLApp::bpp = 0;

SDLApp::SDLApp(int width, int height, int bpp, string caption, bool Mode2D)
{
	this->width = width;
	this->height = height;
	this->bpp = bpp;
	this->caption = caption;
	this->Mode2D =Mode2D;

	flags = 0;

	done = false;

	/*for (int i = 0; i < 323; i++)
		keys[i] = false;*/

	if(!init_SDL())
		done = true;

	onResize = NULL;
	surface = NULL;

	keys =Keys::getInstance();
	mouse =Mouse::getInstance();
}

SDLApp::~SDLApp()
{
	Mix_Quit();
	SDL_Quit();
}

/*
 * Creates the Window with the desired Size
 */
bool SDLApp::createWindow()
{
	if(surface != NULL)
	{
		SDL_FreeSurface(surface);
		surface = NULL;
	}
	if(SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, flags & VSYNC) != 0)
	{
		done = true;
		return false;
	}

	int options = SDL_OPENGL | SDL_RESIZABLE;

	if(width == 0 && height == 0 && bpp == 0)
	{
		const SDL_VideoInfo *vinf = SDL_GetVideoInfo();

		width = vinf->current_w;
		height = vinf->current_h;
		bpp = vinf->vfmt->BitsPerPixel;
	}

	surface = SDL_SetVideoMode(width, height, bpp, options);
	if(!surface)
	{
		done = true;
		return false;
	}

	SDL_WM_SetCaption(caption.c_str(), NULL);

	if(!init_GL())
	{
		done = true;
		return false;
	}

	return true;
}

bool SDLApp::destroyWindow()
{
	return false;
}

bool SDLApp::init_SDL()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		done = true;
		return false;
	}

	Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) ==-1) return false;

	//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	return true;
}

void SDLApp::setOptions(char options)
{
	flags = 0 | options;
}

int SDLApp::getWidth()
{
	return width;
}

int SDLApp::getHeight()
{
	return height;
}

void SDLApp::GetMousePos(int &x, int &y)
{
	x = mouse_x;
	y = mouse_y;
}

bool SDLApp::init_GL()
{
	GLenum err = glewInit();

	if(GLEW_OK != err)
	{
		done = true;
		return false;
	}

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(Mode2D) glOrtho(0, width, 0, height, -1, 1);
	else gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int error = glGetError();
	if(error != GL_NO_ERROR)
	{
		done = true;
		return false;
	}

	return true;
}

bool SDLApp::isRunning()
{
	return !done;
}

void SDLApp::Run()
{
	while(SDL_PollEvent(&Event))
	{
		switch(Event.type)
		{
		case SDL_QUIT:
			done = true;
			break;
		case SDL_KEYUP:
			//keys[Event.key.keysym.sym] = false;
			keys->setKey(Event.key.keysym.sym, false);
			break;
		case SDL_KEYDOWN:
			//keys[Event.key.keysym.sym] = true;
			keys->setKey(Event.key.keysym.sym);
			break;
		case SDL_VIDEORESIZE:
			width = Event.resize.w;
			height = Event.resize.h;

			createWindow();

			glViewport(0, 0, width, height);

			if(onResize != NULL)
				onResize();
			break;
		case SDL_MOUSEMOTION:
			mouse_x = Event.motion.x;
			mouse_y = Event.motion.y;
			mouse->setPosition(Event.motion.x, height - Event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse->setButton(Event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			mouse->setButton(Event.button.button, false);
			break;
		}
	}

	SDL_GL_SwapBuffers();
}

void SDLApp::setOnResizeFunc(void (*func)(void))
{
	onResize = func;
}

void SDLApp::setOnMouseMoveFunc(void(*func)(int, int))
{
	onMouseMove =func;
}

} /* namespace Fumasu */
