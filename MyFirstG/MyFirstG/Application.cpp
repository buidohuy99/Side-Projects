#include "Application.h"
#include "GameObject.h"
#include "SpriteBatch.h"
#include "Shader.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputControl.h"
#include "mainGame.h"
#include <iostream>
#include <exception>

Application* Application::myApplication=nullptr;

Application* Application::createInstance(){
	if(myApplication==nullptr){
		myApplication=new Application;
	}
	return myApplication;
}

Application::Application():currentState(nullptr),currentID(-1){
	isActive=true;
	physicsfps=100.0f;
	fps=0.0f;
	try{
		if(SDL_Init(SDL_INIT_EVERYTHING)<0){
			throw std::runtime_error(SDL_GetError());
		}
		// Set attributes
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); 
		
		//Create 1 window
		window=new Window("My World",640,480);
		
		//load GLAD
		if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
			throw std::runtime_error("Cannot initialize OpenGL");
		}
		//Set viewport to draw to be the new window's viewport
		glViewport(0,0,window->winWidth,window->winHeight);
		
		//aaaa
		//SDL_GL_SetSwapInterval(1);
		
		//ResourceManager
		resourceManager = ResourceManager::createInstance();
		if(resourceManager == nullptr) 
			throw std::runtime_error("Cannot initialize resourceManager"); 
		
		//SceneManager
		sceneManager = SceneManager::getInstance();
		if(sceneManager == nullptr) 
			throw std::runtime_error("Cannot initialize sceneManager"); 

		//InputControl
		inputControl =InputControl::getInstance();
		if(inputControl == nullptr) 
			throw std::runtime_error("Cannot initialize inputControl"); 

	}
	catch(std::runtime_error& error){
		std::cerr<<error.what()<<std::endl;
	}
}
 
void Application::SDLEvtHandling(SDL_Event& e){ //tam thoi co van de
	while(SDL_PollEvent(&e)!=NULL){
		switch(e.type){
		case SDL_QUIT: // X's out of window
			isActive=false;
			break;
		case SDL_WINDOWEVENT: //Event related to window
			{
				if(e.window.event==SDL_WINDOWEVENT_SIZE_CHANGED){ //Window get resize
					glViewport(0,0,e.window.data1,e.window.data2);
				}
			};
			break;
		case SDL_MOUSEMOTION:
            inputControl->setMouseCoords((float)e.motion.x, (float)e.motion.y);
            break;
        case SDL_KEYDOWN:
            inputControl->pressKey(e.key.keysym.sym);
            break;
        case SDL_KEYUP:
            inputControl->releaseKey(e.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            inputControl->pressKey(e.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
			inputControl->releaseKey(e.button.button);
            break;
		}
		if(!isActive) break;
	}
}

void Application::InitContents() {
	if(!currentState) return;
	currentState->InitContents(this);
}

void Application::HandleInput(){
	if(!currentState) return;
	currentState->HandleInput(this);
}

void Application::Update() {
	if(!currentState) return;
	currentState->Update(this);
}

void Application::Draw() {
	if(!currentState) return;
	currentState->Draw(this);
}

void Application::calculateFPS(){
	static const unsigned int samplesNum=1000;
	static unsigned int msPerFrame[samplesNum];
	static unsigned int currentSample=0;

	if(currentSample>=10) currentSample=0;

	static unsigned int prevTick=SDL_GetTicks();
	unsigned int currentTick=SDL_GetTicks();
	unsigned int frameTime=currentTick-prevTick;
	msPerFrame[currentSample]=frameTime;

	prevTick=currentTick;

	int max= currentSample < 10 ? currentSample : samplesNum;
	float avrgTimePerFrame=0;
	for(int i=0;i<max;i++){
		avrgTimePerFrame+=msPerFrame[i]/max;
	}

	if(avrgTimePerFrame>0){
		fps=1000/avrgTimePerFrame;
	}
	currentSample++;
}

void Application::loadAllGameStates() {
	this->addState(new mainGame());
}

void Application::Run(){
	
	//Enable GL Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	//Shader
	resourceManager->LoadShader("Shaders/shader.vert","Shaders/shader.frag","basicShader");
	
	//Init Scene Manager
	sceneManager->initSM(window->winWidth,window->winHeight);
	
	//Set once Projection Matrix
	Shader* currentShader=resourceManager->GetShader("basicShader");
	currentShader->use();
	currentShader->setMatrix4("p",sceneManager->getOrthoMatrix());

	//Load game states
	loadAllGameStates();
		//State change to first game state
	changeState(0);

	//Do Init Content of SceneManager and ResourceManager here
	InitContents();
	
	//set clear color
	glClearColor(0.3f,0.3f,0.7f,0.3f);

	//Initialize fps limiter
	unsigned int prevTicks=0;
	float accumulated=0.0f;
	float physicsTime= 1000.0f/physicsfps;
	
	//Start Game Loop
	while(isActive){
		glClear(GL_COLOR_BUFFER_BIT);
		unsigned int currentTicks = SDL_GetTicks();
		prevTicks = currentTicks;
	//Kiem Tra nut An ve cua so game, thay doi trang thai game
		inputControl->update();
		SDLEvtHandling(inputChecker);
	//---------Use input to affect enviroment------
		HandleInput();

	// cac trang thai cua game
		currentTicks = SDL_GetTicks();
		unsigned int lapse = currentTicks-prevTicks;
		accumulated+=lapse;
		//----------Physics Loop-----------
		while(accumulated>=physicsTime){
			//----Add physics here---------
			accumulated-=physicsTime;
			Update();
		}	
		//----------Render Scene----------
		//use program
		currentShader->use();
		Draw();
		//Update window
		window->Update();
	}
}

bool Application::addState(gameState* state){
	if(!state) return false;
	statesVec.emplace_back(state);
	if(statesVec.size()==1) {currentState = state; currentID = 0;};
	return true;
}

bool Application::changeState(unsigned int stateNum){
	if(stateNum >= statesVec.size()) return false;
	currentState = statesVec[stateNum];
	currentID = stateNum;
	return true;
}

unsigned int Application::getCurrentStateID(){
	return currentID;
}

SceneManager* Application::getSceneManager(){
	return sceneManager;
}

ResourceManager* Application::getResourceManager(){
	return resourceManager;
}

InputControl* Application::getInputControl(){
	return inputControl;
}

Application::~Application(){
	delete window;
	resourceManager->Clear();
}