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
	
}

void mainGame::HandleInput(Application* app){
	InputControl* inputControl = app->getInputControl();
	if(!inputControl) return;
}

void mainGame::Update(Application* app){

}

void mainGame::Draw(Application* app){
	
}
