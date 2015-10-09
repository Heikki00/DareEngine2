#include "Camera.h"
#include "Transform.h"

Camera::Camera(float fov, float as, float zNear, float zFar){
	projection = Math::perspectiveMatrix(fov, as, zNear, zFar);
	this->type = ComponentType::CAMERA;
}


Camera::~Camera(){

}







Matrix4 Camera::getViewProjection(){
	
	Matrix4 tr = Math::translationMatrix(-getTransform()->getTransformedPos());
	Vector3 x = getTransform()->getTRight();
	Vector3 u = getTransform()->getTUp();
	Vector3 f = getTransform()->getTForward();
	Matrix4 rot(x.x, x.y, x.z, 0,
		u.x, u.y, u.z, 0,
		f.x, f.y, f.z, 0,
		0,0,0,1
		);


	//TODO: Make function to DareMath that returns matrix
	return projection * getTransform()->getTransform().inverse();

	
}

void Camera::addToEngine(CoreEngine* engine)
{
	
	engine->getRenderingEngine()->addCamera(this);
}



