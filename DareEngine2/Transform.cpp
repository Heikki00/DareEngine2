#include "Transform.h"
#include <iostream>

Transform::Transform(){
	
	scale = 1;
	
	isChild = false;

	firstTime = true;

}


Transform::~Transform(){
	
}



Matrix4 Transform::getTransform(){
	Matrix4 mat;

	

	mat.translate(pos);
	
	mat *= rot.toMatrix();



	mat.scale(scale);


	if (isChild && firstTime){
		parentMat = parent->getTransform();
		firstTime = false;
	}

	
	return getParentMatrix() * mat;
}

bool Transform::hasChanged()
{

	


		if (isChild && parent->hasChanged())
			return true;
	
	
	
	if (oldPosition != pos){
		return true;
	}
	if (oldRotation != rot){
		return true;
	}

	if (oldScale != scale){
		return true;
	}

	return false;
}

Matrix4 Transform::getParentMatrix()
{
	if (isChild && parent->hasChanged())   {
		parentMat = parent->getTransform();
		
	}
	return parentMat;
}

Vector3 Transform::getTransformedPos()
{
	return Vector3(getParentMatrix() * pos.homogeneous());

}
Quaternion Transform::getTransformedRot()
{
	if (isChild && parent->hasChanged()){
		parentRot = parent->getTransformedRot() * rot;
		return parentRot;
	}
	return parentRot * rot;
}

void Transform::update()
{
	oldPosition = pos;
	oldRotation = rot;
	oldScale = scale;
}


void Transform::setFromMatrix(const Matrix4& mat){


}



#pragma region moveAndRotate
void Transform::offset(const Vector3& direction, float amount)
{
	pos = pos + (amount * direction);
}

void Transform::rotate(const Vector3& axis, float angle, bool local)
{
	
	auto newRot = Quaternion(axis, angle);

	if (local)
		rot = rot * newRot;

	else
		rot = newRot * rot;
}


void Transform::rotate(const Quaternion& val)
{
	rot = val * rot;
}



#pragma endregion


