#include "Window.h"


Window::Window(int width, int height, std::string title){
	if (!SDL_WasInit(SDL_INIT_EVERYTHING)){
			if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
				DebugUtility::log_err("SDL_Init failed during window creation\n");
				return;
		}
	}


	red_bits = 8;
	blue_bits = 8;
	green_bits = 8;
	alpha_bits = 8;
	

	isClosed = false;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, red_bits);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, green_bits);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, blue_bits);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alpha_bits);

	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, red_bits + blue_bits + green_bits);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	glContex = SDL_GL_CreateContext(window);
	

	glewExperimental = GL_TRUE;
	
	GLenum err = glewInit();
	if (err != GLEW_OK){
		DebugUtility::log_err("Failed to init GLEW: %s\n", glewGetErrorString(err));
		
	}
	
	this->width = width;
	this->height = height;
}

void Window::bindAsRenderTarget(){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glViewport(0, 0, width, height);


}


Window::~Window(){
	std::cout << "Destroying window and OpenGL context...  ";
	SDL_GL_DeleteContext(glContex);
	SDL_DestroyWindow(window);
	std::cout << "Done" << std::endl;
}


void Window::swapBuffers(){
	SDL_GL_SwapWindow(window);
}


void Window::updateEvents(){
	SDL_Event e;
	
	while (SDL_PollEvent(&e)){
		if (e.type == SDL_QUIT){
			isClosed = true;
		}
		
	
	}

}





