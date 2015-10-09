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


	Matrix4 m;

	m.rotate(Vector3(0, 1, 0), PI / 2.f);

	m.translate(Vector3(2, 0, 0));

	m.inverse();
	std::cout << m;


	TestGame* game = new TestGame;
	


	CoreEngine mainComponent(800, 600, 60.0, game);
	mainComponent.createWindow("3D Game Engine Development copy");
	
	
	int i = 0;

	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &i);

//	std::cout << "HERE:  " << i << std::endl;


	mainComponent.start();
	
	return 0;
}
