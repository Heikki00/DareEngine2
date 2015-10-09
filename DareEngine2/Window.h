#pragma once
#include <glew.h>
#include <string>
#include <SDL/SDL.h>
#include "DebugUtility.h"
#include <iostream>
#include "Input.h"

class Window
{
public:
	Window(int width, int height, std::string title);
	~Window();

	inline SDL_Window* getWindow(){ return window; }
	inline SDL_GLContext getContex(){ return glContex; }
	
	//Returns true if window should close (for example, alt + f4)
	inline bool isclosed(){ return isClosed; }
	inline std::string getTitle(){ return title; }
	inline int getWidth(){ return width; }
	inline int getHeight(){ return height; }

	
	//Swaps window's buffers (Double buffering)
	void swapBuffers();

	//Updates window events, like moving or input (if binded). Call every frame
	void updateEvents();

	void bindAsRenderTarget();
	
private:
	char red_bits, green_bits, blue_bits, alpha_bits;
	bool isClosed;
	std::string title;
	int width, height;

 
	SDL_Window* window;
	SDL_GLContext glContex;
};

