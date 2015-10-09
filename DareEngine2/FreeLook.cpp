#include "FreeLook.h"
#include "Input.h"
#include "Transform.h"
#include "TestGame.h"
#include "Time.h"
int i = 0;

FreeLook::FreeLook()
{
	this->type = ComponentType::FREELOOK;
}


FreeLook::~FreeLook()
{
}



void FreeLook::doRotation(const Vector3& axis, float angle)
{
	




	




}




void FreeLook::input(float delta) {
	if (Input::getMouseKey(1)) {
		Input::mouseTrap(true);
	}
	if (!Input::isMouseTrapped()) {
		return;
	}
	int x, y;

	Input::getMouseMovement(x, y);

	float sensFactor = 800.f;
	
	float rotY = x / sensFactor;
	float rotX = y / sensFactor;

	getTransform()->rotate(Quaternion(Vector3::WORLD_UP, rotY));

	float FDdot = getTransform()->getForward().dot(Vector3(0, -1, 0));
	if ((FDdot < 0.9f || rotX < 0.f) && (FDdot > -0.9f || rotX > 0.f)){
		getTransform()->rotate(Quaternion(getTransform()->getLeft(), -rotX));
	}
	


	float speed = 0.1f;


	if (Input::getKey(SDL_SCANCODE_LSHIFT)){

		speed *= 10.f;
	}
	else if (Input::getKey(SDL_SCANCODE_LCTRL)){

		speed /= 5.f;
	}

	


	if (Input::getKey(SDL_SCANCODE_W)){
	
		getTransform()->translate(getTransform()->getForward() * speed);

	}
	if (Input::getKey(SDL_SCANCODE_S)){
		getTransform()->translate(getTransform()->getBack() * speed);
	}
	if (Input::getKey(SDL_SCANCODE_A)){
		getTransform()->translate(getTransform()->getLeft() * speed);
	}
	if (Input::getKey(SDL_SCANCODE_D)){
		getTransform()->translate(getTransform()->getRight() * speed);

	}

	if (Input::getKey(SDL_SCANCODE_X)){
		getTransform()->translate(Vector3(0, 1, 0) * speed);
	}
	if (Input::getKey(SDL_SCANCODE_C)){
		getTransform()->translate(Vector3(0, 1, 0) * -speed);
	}




	if (Input::isJustPressed(SDL_SCANCODE_ESCAPE)){
		Input::mouseTrap(false);

	}

	if (Input::getKey(SDL_SCANCODE_F1) && engine){
		engine->stop();
	}

	

	


}

