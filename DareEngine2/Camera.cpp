#include "Camera.h"
#include "Transform.h"

Camera::Camera(float fov, float as, float zNear, float zFar){
	projection = Math::perspectiveMatrix(fov, as, zNear, zFar);
	this->type = ComponentType::CAMERA;
}


Camera::~Camera(){

}







Matrix4 Camera::getViewProjection(){
	
	Matrix4 m;
	

	//TODO: Make function to DareMath that returns matrix
	return projection * getTransform()->getTransform().inverse();

	
}

void Camera::addToEngine(CoreEngine* engine)
{
	
	engine->getRenderingEngine()->addCamera(this);
}



