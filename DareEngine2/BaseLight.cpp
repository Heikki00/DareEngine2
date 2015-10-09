#include "BaseLight.h"



BaseLight::BaseLight(const Vector3& color, float intensity)
{
	this->color = color;
	this->intensity = intensity;

}

BaseLight::~BaseLight(){

}

void BaseLight::addToEngine(CoreEngine* engine)
{
	engine->getRenderingEngine()->addLight(shared_from_this());
}


void BaseLight::setShandowInfo(std::shared_ptr<ShandowInfo> shandowInfo){
	this->shandowInfo = shandowInfo;

}

