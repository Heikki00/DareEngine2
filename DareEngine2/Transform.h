#pragma once
#include <memory>
#include "FestusMath.h"
//#include "Camera.h"


//Class that represents the location, orientation and scale of an object in 3D-space.
//TODO: Maby possibly optimize getWorld*() methods(ecpecially rotation) so they call isDirty (worth it?)
class Transform
{
private:
	Matrix4 cachedTransform;
	bool dirty;

	float scaleNum;
	Quaternion rotation;
	Vector3 translation;

	Transform* parent;

	//Is this ore any of parents dirty?
	bool isDirty();


public:

	//Constructor, inits scaleNum to 1, dirty to false and parent to nullptr, all other to default constructors
	Transform();

	

	//Adds translation to current translation
	void translate(const Vector3& translation);

	//Adds (x,y,z) to current translation
	void translate(float x, float y, float z);

	//Rotates by q. rotation = rotation * q
	void rotate(const Quaternion& q);

	//Creates a Quaternion with axis and an angle, and rotates by that
	void rotate(const Vector3& axis, float angle);

	//Multiplies current scale by f
	void scale(float f);


	//Returns MODEL-SPACE position
	Vector3 getPosition() const;

	//Sets MODEL_SPACE position
	void setPosition(const Vector3& v);

	//Returns MODEL-SPACE rotation
	Quaternion getRotation() const;

	//Sets MODEL-SPACE rotation
	void setRotation(const Quaternion& q);;

	//Returns MODEL-SPACE scale
	float getScale() const;

	//Sets MODEL-SPACE scale
	void setScale(float f);

	//Returns WORLD-SPACE position
	Vector3 getWorldPosition() const;

	//Returns WORLD-SPACE rotation
	Quaternion getWorldRotation() const;

	//Returns WORLD-SPACE scale
	float getWorldScale() const; 

	//Returns vector v ROTATED in MODEL-SPACE
	Vector3 rotateVector(const Vector3& v) const;

	//Returns vector v ROTATED in WORLD-SPACE
	Vector3 rotateVectorWorld(const Vector3& v) const;

	//Returns the translation matrix that translates form model-space to world-space
	Matrix4 getTransform();

	//Sets parent of this translation. Please don't fuck around with this.
	void setParent(Transform* parent);


	Vector3 getForward() const { return rotateVector(Vector3::WORLD_FORW); }
	Vector3 getBack() const { return rotateVector(Vector3::WORLD_BACK); }
	Vector3 getLeft() const { return rotateVector(Vector3::WORLD_LEFT); }
	Vector3 getRight() const { return rotateVector(Vector3::WORLD_RIGHT); }
	Vector3 getUp() const { return rotateVector(Vector3::WORLD_UP); }
	Vector3 getDown() const { return rotateVector(Vector3::WORLD_DOWN); }

	Vector3 getWorldForward() const { return rotateVectorWorld(Vector3::WORLD_FORW); }
	Vector3 getWorldBack() const { return rotateVectorWorld(Vector3::WORLD_BACK); }
	Vector3 getWorldLeft() const { return rotateVectorWorld(Vector3::WORLD_LEFT); }
	Vector3 getWorldRight() const { return rotateVectorWorld(Vector3::WORLD_RIGHT); }
	Vector3 getWorldUp() const { return rotateVectorWorld(Vector3::WORLD_UP); }
	Vector3 getWorldDown() const { return rotateVectorWorld(Vector3::WORLD_DOWN); }
};


