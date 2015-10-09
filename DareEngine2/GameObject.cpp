#include "GameObject.h"
#include "CoreEngine.h"
#include "GameComponent.h"

GameObject::GameObject(){
	transform = new Transform;
}

GameObject::~GameObject(){
	delete transform;
}

void GameObject::addChild(std::shared_ptr<GameObject> child){
	child->getTransform()->setParent(this->transform);
	child->setEngine(engine);
	children.push_back(child);


}


void GameObject::addComponent(std::shared_ptr<GameComponent> component){
	
	component->setParent(this);
	components.push_back(component);

}



void GameObject::input(float delta){
	
	transform->update();
	for (auto component : components){
		component->input(delta);
	}
	
	




}

void GameObject::render(Shader* shader, RenderingEngine* renderingEngine){
	for (auto component : components){
		component->render(shader, renderingEngine);
	}
	


}

void GameObject::update(float delta){
	
	
	
	for (auto component : components){
		component->update(delta);
	}
	


}

void GameObject::inputAll(float delta){

	transform->update();
	this->input(delta);


	for (auto child : children){
		child->input(delta);
	}




}

void GameObject::renderAll(Shader* shader, RenderingEngine* renderingEngine){
	this->render(shader, renderingEngine);

	for (auto child : children){
		child->renderAll(shader, renderingEngine);
	}
}

void GameObject::updateAll(float delta){



	this->update(delta);

	for (auto child : children){
		child->update(delta);
	}

}




void GameObject::setEngine(CoreEngine* engine){
	if (engine != this->engine){
		this->engine = engine;
		for (auto component : components){
			component->addToEngine(engine);
		}

		for (auto child : children){
			child->setEngine(engine);
		}
	}


}

std::vector<std::shared_ptr<GameObject>> GameObject::getAllAttached(){

	std::vector<std::shared_ptr<GameObject>> result;

	result.push_back(shared_from_this());

	for (auto& child : children){
		auto cVec = child->getAllAttached();
		result.insert(result.end(), cVec.begin(), cVec.end());
	}

	
	return result;



}



std::vector<std::shared_ptr<GameObject>> GameObject::getAllChildren(){
	return children;

}


std::shared_ptr<GameObject> GameObject::getChild(int index){
	try{
		return children.at(index);

	}

	catch(...){
		DebugUtility::log_err("ERROR: Tried to retrive child of GameObject %s that was out of boarders\n", name.c_str());
		return nullptr;
	}



}


void GameObject::removeChild(int index){
	if (index < 0 || index >= children.size()){
		DebugUtility::log_err("ERROR: Tried to remove child of GameObject %s that was out of boarders\n", name.c_str());
		return;

	}

	children.at(index)->removeAllChildren();
	children.erase(children.begin() + index);



}


void GameObject::removeAllChildren(){
	
	for (auto child : children){
		child->removeAllChildren();
	}

	children.clear();


}


std::shared_ptr<GameObject> GameObject::findByName(const std::string& name){
	if (this->name == name){
		return shared_from_this();
	}

	for (auto child : children){
		
		auto res = child->findByName(name);
		
			if (res != nullptr){
				return res;
			}

		




	}

	return nullptr;

}


std::vector<std::shared_ptr<GameComponent>>  GameObject::getComponents(){

	return components;
}

std::shared_ptr<GameComponent> GameObject::getComponent(int index){
	try{
		return components.at(index);

	}

	catch (...){
		DebugUtility::log_err("ERROR: Tried to retrive component of GameObject %s that was out of boarders\n", name.c_str());
		return nullptr;
	}


}

void GameObject::removeComponent(int index){

	if (index < 0 || index >= components.size()){
		DebugUtility::log_err("ERROR: Tried to remove component of GameObject %s that was out of boarders\n", name.c_str());
		return;

	}

	
	components.erase(components.begin() + index);







}


void GameObject::removeAllComponents(){
	components.clear();
}

std::vector<std::shared_ptr<GameComponent>> GameObject::getComponetsOfType(ComponentType type){
	
	std::vector<std::shared_ptr<GameComponent>> result;
	for (auto component : components){
		if (component->type == type) result.push_back(component);
	}

	return result;


}










