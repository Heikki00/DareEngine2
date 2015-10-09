#include "Shader.h"
#include <iostream>
#include <SDL\SDL.h>
#include "Window.h"
#include <glew.h>
#include "CoreEngine.h"
#include "DebugUtility.h"
#include "TestGame.h"

#include "DareMath.h"

int main(int argc, char** argv){


	Transform t, t1, t2;
	t1.setParent(&t);
	t2.setParent(&t1);
	t1.translate(Vector3(1, 2, 3));
	std::cout << t2.getTransform();


	TestGame* game = new TestGame;
	


	CoreEngine mainComponent(800, 600, 60.0, game);
	mainComponent.createWindow("3D Game Engine Development copy");
	
	
	int i = 0;

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &i);

//	std::cout << "HERE:  " << i << std::endl;


	mainComponent.start();
	
	return 0;
}
