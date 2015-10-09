#pragma once
#include <SDL/SDL.h>

namespace Time{
	//1000.0
	extern const double SECOND;


	//Returs how much time has passed after sdl initalization(a.k.a start of this program) in milliseconds (0.001s)
	inline long getTime(){ return SDL_GetTicks(); }



}














