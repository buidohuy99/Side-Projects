#include "mainGame.h"
#include "Application.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputControl.h"

mainGame::mainGame(void) 
{
}


mainGame::~mainGame(void)
{
	
}

void mainGame::loadSpritesAndTextures(ResourceManager* resourceManager){
	if(!resourceManager) return;
	//Textures + Sprites
	resourceManager->LoadTexture("Sprites/Overworld.png",GL_TRUE,"Overworld Tex",640,576);
	resourceManager->LoadSprite("Sprites/Overworld.spr","Overworld Tex","Overworld Sprites");
	resourceManager->LoadTexture("Sprites/Character.png",GL_TRUE,"Character Tex",64,128);
	resourceManager->LoadSprite("Sprites/Character.spr","Character Tex","Character Sprites");
}

void mainGame::loadAniFrames(ResourceManager* resourceManager){
	if(!resourceManager) return;
	//Create Aniframes for Character
	unsigned int character[16];
	for(int j=0;j<16;character[j]=j++);
	std::vector<unsigned int> putIn(character,character+16);
	resourceManager->LoadAniFrame("CharWalk","Character Sprites",putIn);
}

void mainGame::loadMaps(SceneManager* sceneManager){
	if(!sceneManager) return;
	//Load Map
	sceneManager->loadMap("Level 1","Map/level1.minf","Map/level1.mdat","Overworld Sprites");
}

void mainGame::InitContents(Application* app){
	ResourceManager* resourceManager = app->getResourceManager();
	loadSpritesAndTextures(resourceManager);
	loadAniFrames(resourceManager);
	SceneManager* sceneManager = app->getSceneManager();
	loadMaps(sceneManager);
}

void mainGame::HandleInput(Application* app){
	InputControl* inputControl = app->getInputControl();
	SceneManager* sceneManager = app->getSceneManager();
	if(!inputControl) return;
	if(inputControl->isKeyDown(SDLK_a)){
		sceneManager->moveCamera(glm::vec2(-1.0f,0.0f));
	}
	if(inputControl->isKeyDown(SDLK_d)){
		sceneManager->moveCamera(glm::vec2(1.0f,0.0f));
	}
	if(inputControl->isKeyDown(SDLK_w)){
		sceneManager->moveCamera(glm::vec2(0.0f,1.0f));
	}
	if(inputControl->isKeyDown(SDLK_s)){
		sceneManager->moveCamera(glm::vec2(0.0f,-1.0f));
	}
}

void mainGame::Update(Application* app){
	
}

void mainGame::Draw(Application* app){
	SceneManager* sceneManager = app->getSceneManager();
	sceneManager->drawScene();
}
