#include "GameComponent.h"
#include "GameObject.h"
#include "CoreEngine.h"

Transform* GameComponent::getTransform()
{
	
	return parent->getTransform();

}

void GameComponent::setParent(GameObject* parent){
	this->parent = parent;
	



}