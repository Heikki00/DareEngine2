#pragma once
#include <memory>
#include "GameObject.h"

class CoreEngine;
class GameObject;

//Interface(parent) for user-defined game. Update and input have default behavior, they update and input root object. 
//However, they are virtual, so they will not be called when overwritten
class Game{
public:

	//Default: updates all GameObjects
	virtual void update(float delta);

	//Default: processes input for all GameObjects
	virtual void input(float delta);

	//Default: none. Called once
	virtual void init();

	//Returns rootObject. If root is null, creates it. USE THIS INSTEAD OF DIRECTLY ACCESSING ROOT
	std::shared_ptr<GameObject> getRootObject();

	//Non.virtual function. Calls setEngine() on rootObject, thus setting engine to all objects and components.
	void setEngine(CoreEngine* engine);

private:
	std::shared_ptr<GameObject> root;

};

