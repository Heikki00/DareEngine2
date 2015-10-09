#include "Game.h"

void Game::update(float delta){
	
	getRootObject()->updateAll(delta);
}

void Game::input(float delta){
	getRootObject()->inputAll(delta);
}



void Game::init()
{

}

std::shared_ptr<GameObject> Game::getRootObject(){
	if (root == nullptr)
		root = GameObject::makePtr();

	return root;
}


void Game::setEngine(CoreEngine* engine){
	getRootObject()->setEngine(engine);
}














