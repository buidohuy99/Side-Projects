#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>

#define SDL_WINDOW_FLAGS SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN

class Window{
private:
	SDL_Window* myWindow;
	SDL_GLContext myGLContext;
public:
	int winWidth,winHeight;
	Window(const std::string& name,int width,int height);
	~Window();
	void Update();

};