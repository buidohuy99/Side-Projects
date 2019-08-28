#include "gameState.h"
#include "Application.h"

gameState::gameState(void){
	
}

gameState::~gameState(void){

}

bool gameState::changeState(std::string State){
	return true;
	//return Application::createInstance()->changeState(State);
}