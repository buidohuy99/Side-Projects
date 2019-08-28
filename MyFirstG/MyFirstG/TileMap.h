#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#define MAX_LAYERS 5

enum class TILE_LAYERS{
	FORE_1,FORE_2,BACK_1,BACK_2,MAIN
};

class GameObject;
class Sprite;
class Camera2D;
class SpriteBatch;
struct ColorRGBA8;
class Physics;

struct Tile{
	int spriteNum;
	float angle;
	bool isFlip;
	Tile(){
		spriteNum = -1;
		angle = 0.0f;
		isFlip = 0;
	}
	void addLayer(unsigned int spriteNum,float angle, bool isFlip){
		if(spriteNum < 0) return;
		this->spriteNum = spriteNum;
		this->angle = angle;
		this->isFlip = isFlip;
	}
};

class TileMap
{
public:
	TileMap(void);
	~TileMap(void);
	bool setTileMapSprite(std::string SpriteName);
	//Read map with loaded tiles
	//Read map with loaded collision masks
	bool readMapFromFile(std::string mapFile,std::string infoFile);
	bool drawTiles(Camera2D* camera, SpriteBatch* spriteBatch);
	void setMapBounds(glm::vec2 screenSize){
		drawOffsetX=mapWidth*TILE_WIDTH <= screenSize.x ? (screenSize.x - mapWidth*TILE_WIDTH)/2 : 0;
		drawOffsetY=mapHeight*TILE_HEIGHT <= screenSize.y ? (screenSize.y - mapHeight*TILE_HEIGHT)/2 : 0;
		mapBoundX.x= 0.0f;
		mapBoundX.y= drawOffsetX*2 + mapWidth*TILE_WIDTH;
		mapBoundY.x= 0.0f;
		mapBoundY.y= drawOffsetY*2 + mapHeight*TILE_HEIGHT;
	}

	//Get
	unsigned int getTileWidth() const{
		return TILE_WIDTH;
	};
	unsigned int getTileHeight() const{
		return TILE_HEIGHT;
	}
	unsigned int getMapWidth() const{
		return mapWidth;
	};
	unsigned int getMapHeight() const{
		return mapHeight;
	}
	glm::vec2 getMapXBounds(){
		return mapBoundX;
	}
	glm::vec2 getMapYBounds(){
		return mapBoundY;
	}
	glm::vec2 getSpawnSpot(){
		return spawnSpot;
	}
	const std::vector<unsigned int>& getSpriteMap(int layer) const{
		return spriteMap[layer];
	}
	unsigned int getColMask(int x, int y);
	unsigned int getColMask(unsigned int tile);
private:
	//Load spritesheet truoc cho tilemap
	Sprite* mapSprite;
	//Spawn spot for entering
	glm::vec2 spawnSpot;
	//Hinh ve cho cac tile
	std::vector<Tile> tileInfos;
	//Map cua cac hinh ve cho cac tile
	std::vector<unsigned int> spriteMap [MAX_LAYERS];
	//Map cua cac collision mask da duoc dinh nghia
	std::vector<unsigned int> collisionMap;
	
	//So dong va so cot
	unsigned int mapWidth, mapHeight;
	
	//Load Tile info before load map
	//Load collision masks cua cac loai gach muon xay dung
	bool loadInfo(std::string infoFile);
	
	//Load map chua cac hinh ve tileInfo da load
	//Load map chua cac collision masks da dinh nghia o tren
	bool loadMap(std::string mapFile);

	unsigned int drawOffsetX,drawOffsetY;
	glm::vec2 mapBoundX,mapBoundY;

	//Thong tin co ban ve 1 vien gach
	static unsigned int TILE_WIDTH,TILE_HEIGHT;
	static ColorRGBA8 generalCol;
};

//ColisionMap:
// Using binary number
//
// ---------	0001 => right side collidable
// |	   |	0010 => down side collidable
// |	   |	0100 => left side collidable
// |	   |	1000 => up side collidable
// ---------	0011 => right and down sides collidable and so forth...

