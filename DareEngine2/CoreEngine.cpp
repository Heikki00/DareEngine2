#include "CoreEngine.h"
#include "Time.h"
#include "Game.h"
#include <iostream>



Window* CoreEngine::window;

CoreEngine::CoreEngine(int width, int height, double framerate, Game* game): frameRate(framerate){
	DebugUtility::restart_log();
	

	this->width = width;
	this->height = height;



	frameTime = frameRate / Time::SECOND;
	
	game->setEngine(this);
	this->game = game;
	

	isRunning = false;

}


CoreEngine::~CoreEngine()
{
	
	delete game;
	
	delete renderingEngine;
	delete window;
}

void CoreEngine::createWindow(std::string title){
	
	


	CoreEngine::window = new Window(width, height, title);
	window->bindAsRenderTarget();
	



	renderingEngine = new RenderingEngine;
	
	
	game->init();
//	game->setEngine(this);


	
}





void CoreEngine::run(){
	isRunning = true;
	
	int frames = 0;

	
	long prevTime = Time::getTime();
	double unprosessedTime = 0;
	long t = 0;
	float delta = 0;


	while (isRunning){
	
		DebugUtility::check_opengl_error();




		long startTime = Time::getTime();
		delta = startTime - prevTime;
		t += (long)delta;
		prevTime = startTime;
	
		
	
			
			game->input(delta);
			game->update(delta);
			
			frames++;
			Input::update();
			

	
	
		if (t >= Time::SECOND){
			//std::cout << frames << std::endl;
			frames = 0;
			t = 0;
		}
		
		
		if (window->isclosed()){
			stop();
		}

	
			window->updateEvents();

		

			
			renderingEngine->render(game->getRootObject().get());
			
	
			
			

	}


}


void CoreEngine::start(){
	if (isRunning){
		return;
	}
	
	run();
}


void CoreEngine::stop(){
	isRunning = false;
}





RenderingEngine* CoreEngine::getRenderingEngine(){
	if (renderingEngine) return renderingEngine;

	DebugUtility::log_err("ERROR: Asked renderingEngine pointer from CoreEngine when renderingEngine had not been created yet!\n");
	return nullptr;

}

PhysicsEngine* CoreEngine::getPhysicsEngine(){
	if (physicsEngine) return physicsEngine;

	DebugUtility::log_err("ERROR: Asked physicsEngine pointer from CoreEngine when physicsEngine had not been created yet!\n");
	return nullptr;



}


