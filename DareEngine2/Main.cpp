#include "Shader.h"
#include <iostream>
#include <SDL\SDL.h>
#include "Window.h"
#include <glew.h>
#include "CoreEngine.h"
#include "DebugUtility.h"
#include "TestGame.h"
#include "FestusMath.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>



int main(int argc, char** argv){



	
	Math::initMemory(1000, 500, 100, 100);


	



	TestGame* game = new TestGame;
	


	CoreEngine mainComponent(800, 600, 60.0, game);
	mainComponent.createWindow("3D Game Engine Development copy");
	
	CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();
		
	//CEGUI::GUIContext& g = CEGUI::System::getSingleton().createGUIContext(myRenderer.getDefaultRenderTarget());
	//CEGUI::Window* root = CEGUI::WindowManager::getSingleton().createWindow("defaultWindow", "root");
//	g.setRootWindow(root);




	mainComponent.start();
	
	return 0;
}
