#include "Input.h"

namespace Input{

	
	std::vector<bool> oldKeys = std::vector<bool>(1000, false);


	std::vector<bool> mouseOldKeys = std::vector<bool>(50, false);
	unsigned long long timesUpdated = 0;
	unsigned long long mouseLastRequest = 0;

	int mouseX, mouseY;




	

	bool Input::isJustPressed(int key){
		if (oldKeys.at(key)){
			std::cout << "k"<< std::endl;
			return false;
		}


		int i = 0;
		auto keyBoard = SDL_GetKeyboardState(&i);
		return keyBoard[key];




	}

	void Input::update(){
		invalidateJustPressed();
		timesUpdated++;
	}


	void Input::invalidateJustPressed(){
		int numKeys = 0;
		auto currentKeyboard = SDL_GetKeyboardState(&numKeys);

		for (int i = 0; i < numKeys; i++){
			oldKeys.at(i) = currentKeyboard[i];



		}

		for (int i = 0; i < 5; i++){
			mouseOldKeys.at(i) = (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(i));
		}

	}


	bool Input::getKey(int key){
		
			int i = 0;
			auto keyBoard =  SDL_GetKeyboardState(&i);
			return keyBoard[key];
		
		
		
	}


	bool Input::getMouseKey(int key){
			
		if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(key)){
			return true;
		}
		return false;
	
	}

	



	bool Input::isJustMousePressed(int key){
		if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(key)){
			if (mouseOldKeys.at(key)){
				return false;
			}

			return true;

		}



	return false;

	}


	void Input::getMouseMovement(int& x, int& y){
	if (mouseLastRequest == timesUpdated){
			x = mouseX;
			y = mouseY;
			return;
	}
		SDL_GetRelativeMouseState(&x, &y);
		mouseX = x;
		mouseY = y;
		mouseLastRequest = timesUpdated;
	}

	void mouseTrap(bool trap){
		SDL_SetRelativeMouseMode(SDL_bool(trap));
	}



}
