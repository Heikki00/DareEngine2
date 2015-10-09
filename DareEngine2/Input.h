#pragma once
#include <iostream>
#include <vector>
#include "DebugUtility.h"
#include <SDL/SDL.h>



namespace Input{



	//Returns true if given mouse key is pressed
	bool getMouseKey(int key);

	//Returns true if given keyboard key is pressed
	bool getKey(int key);
	
	//Returns true if given keyboard key is pressed in this frame
	bool isJustPressed(int key);

	//Returns true if given mouse key is pressed in this frame
	bool isJustMousePressed(int key);

	//Puts cursors relative movement to parameters. Bolth will be 0 if cursor is outside the window
	void getMouseMovement(int& x, int& y);

	//Traps the cursor in window, so cursor is invisible and cannot leave the window.Trap == true for capturing mouse, false for releasing it;
	void mouseTrap(bool trap);

	//Returns true if mouse is trapped to window, false otherwise
	inline bool isMouseTrapped(){ return SDL_GetRelativeMouseMode() == SDL_TRUE; }


	//Updates input object, so isJustPressed etc. will work. Called every frame (hopefully...)
	void update();



	
	
	extern std::vector<bool> oldKeys;

	
	extern std::vector<bool> mouseOldKeys;

	extern int mouseX, mouseY;

	void invalidateJustPressed();

	extern unsigned long long timesUpdated;
	extern unsigned long long mouseLastRequest;
	
}

