#include "Application.h"

int main(int argc, char* args[]){
	Application* app=Application::createInstance();
	app->Run();
	delete app;
	SDL_Quit();
	return 0;
}
