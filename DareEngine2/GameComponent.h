#pragma once
#include <memory>
#include "Transform.h"

#define GAMECOMPONENTH


class Shader;
class CoreEngine;
class RenderingEngine;
class GameObject;
class Transform;

enum ComponentType{
	UNKNOWN = 0,
	MESH,
	DIRECTIONALLIGHT,
	POINTLIGHT,
	SPOTLIGHT,
	CAMERA,
	FREELOOK,
	MESHRENDERER
};




//Interface for user-defined components that can be added to GameObject.
class GameComponent{
public:
	GameComponent(){ type = ComponentType::UNKNOWN; };

	virtual ~GameComponent(){};

	//Virtual function, used to handle input. Does not have to be overridden.
	virtual void input(float delta){}
	
	//Virtual function, update component. Does not have to be overridden.
	virtual void update(float delta){
	
	
	
	
	}




	//Virtual function, used in rendering. Does not have to be overridden. Shader is the shader to be used in rendering.
	virtual void render(Shader* shader, RenderingEngine* renderingEngine){}


	
	void setParent(GameObject* parent);
	inline GameObject* getParent() const { return parent; }

	//Shortcut to getParent()->getTransform()
	Transform* getTransform();
	
	//Virtual function, that gives component pointer to CoreEngine. Does not have to be overridden
	virtual void addToEngine(CoreEngine* engine){}

	ComponentType type;
	

private:

	GameObject* parent;

};





