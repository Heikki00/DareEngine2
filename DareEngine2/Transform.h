#pragma once
#include <memory>
#include "DareMath.h"
//#include "Camera.h"



class Transform
{
public:
	Transform();
	~Transform();

	Matrix4 getTransform();
	
	

	inline Vector3 getPosition() const { return pos; }
	inline void setPosition(const Vector3& translation){ pos = translation; }
	inline void setPosition(float x, float y, float z){ pos = Vector3(x, y, z); }
	void offset(const Vector3& direction, float amount);

	
	inline Quaternion getRotation() const { return rot; }
	inline void setRotation(const Quaternion& val){ rot = val; }
	
	void rotate(const Vector3& axis, float angle, bool local = true);
	void rotate(const Quaternion& val);
	

	inline float getScale() const { return scale; }
	inline void setScale(float scale){ this->scale = scale; }
	
	inline void setParent(Transform* transform){ 
		this->parent = transform;	
		isChild = true; 		
	}
	
	


	void setFromMatrix(const Matrix4& mat);
	Vector3 getTransformedPos();
	Quaternion getTransformedRot();



	inline Vector3 getForward() const { return rot.rotateVector(Vector3::WORLD_FORW); }
	inline Vector3 getBackward() const { return rot.rotateVector(Vector3::WORLD_BACK); }
	inline Vector3 getLeft() const { return rot.rotateVector(Vector3::WORLD_LEFT); }
	inline Vector3 getRight() const { return rot.rotateVector(Vector3::WORLD_RIGHT); }
	inline Vector3 getUp() const { return rot.rotateVector(Vector3::WORLD_UP); }
	inline Vector3 getDown() const { return rot.rotateVector(Vector3::WORLD_DOWN); }

	inline Vector3 getTForward() { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_FORW)); }
	inline Vector3 getTBackward() { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_BACK)); }
	inline Vector3 getTLeft() { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_LEFT)); }
	inline Vector3 getTRight()  { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_RIGHT)); }
	inline Vector3 getTUp()  { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_UP)); }
	inline Vector3 getTDown()  { return getTransformedRot().rotateVector(rot.rotateVector(Vector3::WORLD_DOWN)); }

	void update();

	inline static std::shared_ptr<Transform> makePtr(){ return std::make_shared<Transform>(); }

private:
	Vector3 pos;
	Quaternion rot;
	float scale;
	
	Transform* parent;
	Matrix4 parentMat;
	Quaternion parentRot;
	
	bool isChild;

	

	Vector3 oldPosition;
	Quaternion oldRotation;
	float oldScale;
	
	Matrix4 getParentMatrix();
	bool hasChanged();


	bool firstTime;
};


