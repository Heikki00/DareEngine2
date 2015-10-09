#pragma once

#include "Window.h"
#include "RenderingEngine.h"



class Game;
class RenderingEngine;
class PhysicsEngine;
//Core of the game engine, main loop is located here. Creates rendering engine, updates the game
class CoreEngine
{
public:
	

	CoreEngine(int width, int height, double framerate, Game* game);
	~CoreEngine();

	//starts the game engine
	void start();

	//Stops the game engine
	void stop();
	
	//Creates the window, OpenGL context and RenderingEngine
	void createWindow(std::string title);
	

	inline Window* getWindow(){ return CoreEngine::window; }
	

	RenderingEngine* getRenderingEngine();
	PhysicsEngine* getPhysicsEngine();


	//Pointer to window, might be null
	static Window* window;



private:
	bool isRunning;

	void run();
	
	Game* game;
	RenderingEngine* renderingEngine;
	PhysicsEngine* physicsEngine;

	const double frameRate;
	double frameTime;
	
	int width, height;


};



