#include "SceneManager.h"
#include "Camera2D.h"
#include "SpriteBatch.h"
#include "TileMap.h"
#include <iostream>
#include "Player.h"

Camera2D* SceneManager::myCamera=nullptr;
SceneManager* SceneManager::SManager=nullptr;

bool SceneManager::renderCurrentMap(){
	return currentTileMap->drawTiles(myCamera,gameBatch);
}

bool SceneManager::renderObjInView(){
	return true;
}

SceneManager::SceneManager(void)
{
	gameBatch=nullptr;
	currentTileMap=nullptr;
}

glm::mat4 SceneManager::getOrthoMatrix(){
	return myCamera->getOrthoMatrix();
}

void SceneManager::drawScene(){
	if(updateCamera()) {
		gameBatch->begin();
		if(renderCurrentMap()&&renderObjInView()){
			gameBatch->end();
			gameBatch->renderBatch();
		}
	}
}

bool SceneManager::initSM(float screenWidth,float screenHeight){
	if(!initCamera(screenWidth,screenHeight)) return false;
	gameBatch= new SpriteBatch;
	if(gameBatch == nullptr) {return false;}
	gameBatch->init();
	return true;
}

bool SceneManager::initCamera(float screenWidth,float screenHeight){
	//Camera
	myCamera = Camera2D::instance();
	if(myCamera == nullptr)
		return false;
	return myCamera->initCam(screenWidth,screenHeight);
}

SceneManager::~SceneManager(void)
{
	delete gameBatch;
}

bool SceneManager::loadMap(std::string mapName, std::string mapInfoFile, std::string mapLayoutFile, std::string mapSprite){
	auto found=tileMaps.find(mapName);
	if(found!=tileMaps.end()) return false;
	TileMap* temp= new TileMap;
	if(temp==nullptr) return false;
	if(!temp->setTileMapSprite(mapSprite)) return false;
	if(!temp->readMapFromFile(mapLayoutFile,mapInfoFile)) return false;
	tileMaps.insert(std::pair<std::string,TileMap*> (mapName,temp));
	if(tileMaps.size()==1) {
		setCurrentMap(mapName);
		currentMapName = mapName;
	}
	return true;
}

bool SceneManager::setCurrentMap(std::string mapName){
	if(myCamera==nullptr){
		std::cout<<"Camera not init"<<std::endl;
		return false;
	}
	auto found=tileMaps.find(mapName);
	if(found==tileMaps.end()) return false;
	currentMapName = mapName;
	currentTileMap=found->second;
	currentTileMap->setMapBounds(myCamera->getScreenSizes());
	return true;
}

bool SceneManager::moveCamera(glm::vec2 doDoiVector){
	if(currentTileMap==nullptr) return false;
	myCamera->moveCam(doDoiVector);
	return true;
}

bool SceneManager::zoomCamera(float zoomFactor){
	if(currentTileMap==nullptr) return false;
	myCamera->zoomCam(zoomFactor);
	currentTileMap->setMapBounds(myCamera->getScreenSizes());
	return true;
}

bool SceneManager::updateCamera(){
	if(currentTileMap==nullptr) return false;
	//Prevent from moving out of Map Bounds
	if(myCamera->getUpdatedStatus()){
		glm::vec2 mapXBounds=currentTileMap->getMapXBounds();
		glm::vec2 mapYBounds=currentTileMap->getMapYBounds();
		glm::vec2 screenSize = myCamera->getScreenSizes();
		mapXBounds.x += screenSize.x/2;
		mapXBounds.y -= screenSize.x/2;
		mapYBounds.x += screenSize.y/2;
		mapYBounds.y -= screenSize.y/2;
		glm::vec2 myPos=myCamera->getPosition();
		myCamera->setPosition(myPos);
		if(myPos.x<mapXBounds.x) {myCamera->setPosition(glm::vec2(mapXBounds.x,myPos.y));}
		else if(myPos.x>mapXBounds.y) {myCamera->setPosition(glm::vec2(mapXBounds.y,myPos.y));}
		if(myPos.y<mapYBounds.x) {myCamera->setPosition(glm::vec2(myCamera->getPosition().x,mapYBounds.x));}
		else if(myPos.y>mapYBounds.y) {myCamera->setPosition(glm::vec2(myCamera->getPosition().x,mapYBounds.y));}
	}
	//After recentering, update camera
	myCamera->update();
	return true;
}

TileMap* SceneManager::getCurrentMap(){
	return currentTileMap;
}

std::string SceneManager::getCurrentMapName(){
	return currentMapName;
}