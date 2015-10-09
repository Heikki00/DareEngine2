
#include "GameComponent.h"
#ifdef GAMECOMPONENTH

#pragma once
#include "DareMath.h"

#include <memory>
#include "CoreEngine.h"

class RenderingEngine;

class Camera : public GameComponent
{
public:
	
	
	Camera(float fov, float as, float zNear, float zFar);
	~Camera();

	//Returns projected matrix with projection and cameras Transform
	Matrix4 getViewProjection();


	inline Matrix4 getProjection(){ return projection; }
	inline void setProjection(const Matrix4& val){ this->projection = val; }

	void addToEngine(CoreEngine* renderingEngine) override;

	

	inline static std::shared_ptr<Camera> makePtr(float fov, float as, float zNear, float zFar){ return std::make_shared<Camera>(fov, as, zNear, zFar); }
private:
	
	Matrix4 projection;

};


#endif
