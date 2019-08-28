#pragma once

#include <map>
#include <glm/glm.hpp>
class TileMap;
class GameObject;
class SpriteBatch;
class Camera2D;
class Player;
//Pending object related functions an stufz
//Object will be stored in tree data structure for drawing, currently will use vector

class SceneManager
{
public:
	static SceneManager* getInstance(){
		if(SManager==nullptr) {
			SManager=new SceneManager;
		}
		return SManager;
	}
	~SceneManager(void);

	bool initSM(float screenWidth,float screenHeight);

	//TileMap Related
	bool loadMap(std::string mapName, std::string mapInfoFile, std::string mapLayoutFile, std::string mapSprite);
	bool setCurrentMap(std::string mapName);
	TileMap* getCurrentMap();
	std::string getCurrentMapName();
	
	//Object Related - Pending
	bool addObj(GameObject* newObj);

	//Update Camera->Draw 
	//(draw order : Background -> Tile Map -> Objects)
	void drawScene();

	bool moveCamera(glm::vec2 doDoiVector);
	bool zoomCamera(float zoomFactor);	
	glm::mat4 getOrthoMatrix();

private:
	static SceneManager* SManager;
	SceneManager();
	static Camera2D* myCamera;
	SpriteBatch* gameBatch;

	//Camera Related
	bool initCamera(float screenWidth,float screenHeight);
	bool updateCamera();
	bool checkObjInView(GameObject* obj);

	//TileMap Related
	bool renderCurrentMap();
	std::map<std::string,TileMap*> tileMaps;
	TileMap* currentTileMap;
	std::string currentMapName;
	
	//Object Related - Pending
	bool renderObjInView();
};

