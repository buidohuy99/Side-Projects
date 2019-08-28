#include "Window.h"
#include <iostream>
#include <exception>

Window::Window(const std::string& name,int width,int height){
	//Init Window
	myWindow=SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FLAGS);
	if(myWindow==nullptr){
		throw std::runtime_error(SDL_GetError());
	}
	winWidth=width;winHeight=height;
	//Init GL Context tied to Window
	myGLContext = SDL_GL_CreateContext(myWindow);
	if(myGLContext==nullptr){
		throw std::runtime_error(SDL_GetError());
	}
}

Window::~Window(){
	SDL_GL_DeleteContext(myGLContext);
	SDL_DestroyWindow(myWindow);
}

void Window::Update(){
	SDL_GL_SwapWindow(myWindow);
}
