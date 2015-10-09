#include "Transform.h"
#include <iostream>

Transform::Transform(): scaleNum(1.f), dirty(false), parent(nullptr)
{

}




void Transform::translate(float x, float y, float z)
{
	translation.x += x; translation.y += y; translation.z += z;
	dirty = true;
}

void Transform::translate(const Vector3& translation)
{
	this->translation += translation;
	dirty = true;
}



void Transform::rotate(const Vector3& axis, float angle)
{
	rotation = Quaternion(axis, angle) * rotation;
	dirty = true;
}

void Transform::rotate(const Quaternion& q)
{
	rotation =  q * rotation;
	dirty = true;
}

void Transform::scale(float f)
{
	this->scaleNum *= f;
	dirty = true;
}



Vector3 Transform::getPosition() const
{
	return translation;
}


void Transform::setPosition(const Vector3& v)
{
	translation = v;
	dirty = true;
}


Quaternion Transform::getRotation() const
{
	return rotation;
}


void Transform::setRotation(const Quaternion& q)
{
	rotation = q;
	dirty = true;
}

float Transform::getScale() const
{
	return this->scaleNum;
}

void Transform::setScale(float f)
{
	this->scaleNum = f;
	dirty = true;
}

Vector3 Transform::getWorldPosition() const
{
	Vector3 parentPosition;
	if (parent) parentPosition = parent->getWorldPosition();


	return parentPosition + translation;


}

Quaternion Transform::getWorldRotation() const
{
	Quaternion parentRotation;
	if (parent) parentRotation = parent->getWorldRotation();


	return rotation * parentRotation;
}

float Transform::getWorldScale() const
{
	float parentScale;
	if (parent) parentScale = parent->getWorldScale();


	return scaleNum * parentScale;
}

Vector3 Transform::rotateVector(const Vector3& v) const
{
	return rotation * v;
}

Vector3 Transform::rotateVectorWorld(const Vector3& v) const
{
	return getWorldRotation() * v;
}

Matrix4 Transform::getTransform()
{
	if (!parent) {
		if (!dirty) {
			return cachedTransform;
		}
		
		cachedTransform = Math::translationMatrix(translation);
		cachedTransform *= rotation.toMatrix();
		cachedTransform *= Math::scaleMatrix(scaleNum);
		
		dirty = false;
		return cachedTransform;
	}


	if (dirty || parent->isDirty()) {
		
		Matrix4 parentTransform = parent->getTransform();

		cachedTransform = Math::translationMatrix(translation);
		cachedTransform *= rotation.toMatrix();
		cachedTransform *= Math::scaleMatrix(scaleNum);
		cachedTransform = parentTransform * cachedTransform;

		dirty = false;
		return cachedTransform;
	}
	else {
		return cachedTransform;
	}









}


void Transform::setParent(Transform* parent)
{
	this->parent = parent;
	dirty = true;
}

bool Transform::isDirty() {

	if (dirty) return true;

	if (parent) return parent->isDirty();

	return false;

}


