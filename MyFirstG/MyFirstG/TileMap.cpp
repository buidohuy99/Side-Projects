#include "TileMap.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Camera2D.h"
#include "Vertex.h"
#include "Rect.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Physics.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

unsigned int TileMap::TILE_WIDTH=16;
unsigned int TileMap::TILE_HEIGHT=16;
ColorRGBA8 TileMap::generalCol= ColorRGBA8(255,255,255,255);

TileMap::TileMap(void)
{
	mapSprite=nullptr;
	drawOffsetX=0;
	drawOffsetY=0;
}


TileMap::~TileMap(void)
{
}

//Luu y: ve tile chi khi Camera duoc updated
bool TileMap::drawTiles(Camera2D* camera, SpriteBatch* spriteBatch){
	
	glm::vec2 screenSize = camera->getScreenSizes();
	glm::vec2 bottomLeft = camera->getPosition() - glm::vec2(screenSize.x/2,screenSize.y/2);
	glm::vec2 topRight = bottomLeft + screenSize;

	setMapBounds(screenSize);

	if(topRight.x<=mapBoundX.x || bottomLeft.x>=mapBoundX.y || topRight.y<=mapBoundY.x || bottomLeft.y>=mapBoundY.y) {
		return false;
	}
	
	unsigned int leftX = mapWidth*TILE_WIDTH <= screenSize.x || bottomLeft.x < 0 ? 0 : floor(bottomLeft.x/TILE_WIDTH) ,
		rightX = mapWidth*TILE_WIDTH <= screenSize.x || topRight.x >= (mapWidth-1)*TILE_WIDTH ? mapWidth-1 : ceil(topRight.x/TILE_WIDTH) ;
	unsigned int bottomY = mapHeight*TILE_HEIGHT <= screenSize.y || bottomLeft.y < 0 ? 0 : floor(bottomLeft.y/TILE_HEIGHT),
		topY= mapHeight*TILE_HEIGHT <= screenSize.y || topRight.y >= (mapHeight-1)*TILE_HEIGHT ? mapHeight-1 :ceil(topRight.y/TILE_HEIGHT);
	
	Rect tempDest;
	Rect* tempTex;
	tempDest.setSpan(TILE_WIDTH,TILE_HEIGHT);
	for(unsigned int i = bottomY ; i<=topY ; i++){
		tempDest.position.y = drawOffsetY + i*TILE_HEIGHT;
		for(unsigned int j = leftX ; j<=rightX; j++){
			tempTex = nullptr;
			tempDest.position.x = drawOffsetX + j*TILE_WIDTH;
			for(unsigned int a = 0 ; a<MAX_LAYERS ;a++){
				unsigned int found = spriteMap[a][(mapHeight-1-i)*mapWidth + j];
				int Depth;
				//Switch statement for Depth depending on a layer
				switch (a){
					case TILE_LAYERS::BACK_1: Depth = mapHeight * TILE_HEIGHT + 1; break;
					case TILE_LAYERS::BACK_2: Depth = mapHeight * TILE_HEIGHT + 2; break;
					case TILE_LAYERS::MAIN: Depth = i*TILE_HEIGHT; break;
					case TILE_LAYERS::FORE_1: Depth = -2; break;
					case TILE_LAYERS::FORE_2: Depth = -1; break;
				}
				//Draw by Depth
				if(found!=0) {
					tempTex = mapSprite->findSubSprite(tileInfos[found-1].spriteNum);
					spriteBatch->draw(tempDest,*tempTex,tileInfos[found-1].isFlip,mapSprite->getTex(),Depth,generalCol,tileInfos[found-1].angle,glm::vec2(1.0f,1.0f));		
				}	
			}
		}
		
	}
	return true;
};

bool TileMap::setTileMapSprite(std::string SpriteName){
	mapSprite=ResourceManager::createInstance()->GetSprite(SpriteName);
	if(mapSprite==nullptr) return 0;
	return 1;
}

bool TileMap::readMapFromFile(std::string mapFile,std::string infoFile){
	//find Extension minf
	size_t lastdot=infoFile.rfind('.',infoFile.length());
	if(lastdot==std::string::npos) return false;
	if((infoFile.substr(lastdot+1,infoFile.length()-1)).compare("minf")!=0) return false;
	//If ok load infos
	if(!loadInfo(infoFile)) return false;
	//find Extension mdat
	lastdot=infoFile.rfind('.',infoFile.length());
	if(lastdot==std::string::npos) return false;
	if((infoFile.substr(lastdot+1,infoFile.length()-1)).compare("minf")!=0) return false;
	//If ok load maps
	if(!loadMap(mapFile)) return false;
	return true;
}

bool TileMap::loadInfo(std::string infoFile){
	if(mapSprite==nullptr) 
	{	
		std::cout<<"mapSprite not loaded"<<std::endl;
		return false;
	}
	std::ifstream info;
	info.exceptions(std::ifstream::badbit|std::ifstream::failbit);
	try{
		std::string tempLine;
		info.open(infoFile);
		
		bool isTakingType=true, startInput=false;
		unsigned char phase;
		unsigned int numOfInfos,iterator=0,prevSize;
		
		while(std::getline(info,tempLine)){
			if(isTakingType==true){
				if(tempLine.compare("tileInfo")==0) {phase=0;} 
				else if(tempLine.compare("")==0) {continue;}
				else {throw std::string("Undefined type of Info found in input file called : ")+tempLine;};
				isTakingType=false;
			}else
			{
				std::stringstream ss(tempLine);
				std::string checker;	
				ss>>checker;
				if(!startInput){	
					if(checker.compare("start")==0) {
						iterator = 0;
						numOfInfos = 0;
						if(tempLine.compare("start")==0) {			
							throw std::string("Wrong start command");
						}
						ss>>numOfInfos;
						if(phase==0)
							{
								prevSize = tileInfos.size();
								tileInfos.resize(tileInfos.size()+numOfInfos);
							}
						startInput=true;
					}
				}
				else
				{
					if(checker.compare("end")==0) {
						if(iterator<numOfInfos) {
							if(phase==0)
								tileInfos.resize(prevSize+iterator);
						}
						startInput=false;
						isTakingType=true;
					}
					else
					{
						if(checker.compare("")==0) continue;
						if(checker.compare(">")!=0) throw std::string("Didn't find input symbol");
						if(iterator>=numOfInfos) {
							startInput=false;
							isTakingType=true;
							continue;
						}
						switch(phase){
						case 0:
							while(true){
								int subSpriteNum = -1;
								float angle = -1.0f; int isFlip=-1;
								ss>>subSpriteNum>>angle>>isFlip;
								if(subSpriteNum<0 || angle<0.0f || isFlip<0) break;
								if(!mapSprite->findSubSprite(subSpriteNum)) {throw std::string("subSprite not found: " + subSpriteNum);} 
								tileInfos[prevSize+iterator].addLayer(subSpriteNum,angle,(bool) isFlip);
							}
							break;
						};
						iterator++;
					}
				}
			}
			tempLine.clear();
		}
	}
	catch(std::ifstream::failure e)
	{
		
		if(!info.eof()){
			info.close();
			if(tileInfos.size()==0) {
			std::cout<<"tileInfos not Initialized"<<std::endl;
			}
			std::cout<<"Failure to load infoFile: "<<infoFile<<std::endl;
			return false;
		}
	}
	catch(std::string str){
		info.close();
		std::cout<<str<<std::endl;
		return false;
	}
	info.close();
	return true;
}

bool TileMap::loadMap(std::string mapFile){
	std::ifstream map;
	map.exceptions(std::ifstream::badbit|std::ifstream::failbit);
	try{
		map.open(mapFile);
		std::string tempLine;
	    bool i=0;
		unsigned char phase=0;
		bool isTakingType=true;
		int currentLayer = -1;
		while(std::getline(map,tempLine)){
			std::stringstream ss(tempLine);
			std::string check;
			ss>>check;
				if(!i){
					if(check.compare("")==0) continue;
					if(check.compare(">")!=0) throw std::string("Didn't find input symbol");
					mapWidth=mapHeight=0;
					ss>>mapWidth>>mapHeight;
					//Init spawn point (bottom left of Obj!!!!)
					check.clear();
					ss>>check;
					if(check.compare("spawnPoint")!=0)  throw std::string("Didn't find spawn Point");
					float spawnX=-2.0f,spawnY=-2.0f;
					ss>>spawnX>>spawnY;
					if(spawnX < 0.0f || spawnY < 0.0f || spawnX > mapWidth*TILE_WIDTH || spawnY > mapHeight*TILE_HEIGHT) throw std::string("Didn't find spawn Point");
					spawnSpot = glm::vec2(spawnX,spawnY);
					i = true;
					if(mapWidth==0||mapHeight==0) {
						throw std::string("Obsolete width/height: ")+tempLine;
					}
				}else{
					if(isTakingType==true){
						if(check.compare("spriteMap")==0) {phase=0;
							currentLayer = -1;	
							ss>>currentLayer;
							if(currentLayer < 0 || currentLayer>=MAX_LAYERS) { throw std::string("SpriteMap Layer not indicated or Layer not available : ")+tempLine; };
						} 
						else if(check.compare("collisionMap")==0) {phase=1;}
						else if(check.compare("")==0) {continue;}
						else {throw std::string("Undefined type of Info found in input file called : ")+tempLine;};
						isTakingType=false;
					}else
					{
						if(check.compare("")==0) continue;
						if(check.compare("<")!=0) {
							if(check.compare("spriteMap")==0) {phase=0;
								currentLayer = -1;
								ss>>currentLayer;
								if(currentLayer < 0 || currentLayer>=MAX_LAYERS) { throw std::string("SpriteMap Layer not indicated or Layer not available : ")+tempLine; };
							} 
							else if(check.compare("collisionMap")==0) {phase=1;}
							else {throw std::string("Undefined type of Info found in input file called : ")+tempLine;};
							continue;
						}
						switch(phase){
						case 0:
							if(spriteMap[currentLayer].size()+mapWidth>mapHeight*mapWidth) {
								throw std::string("Violated predetermined height: ")+tempLine;
							};
							break;
						case 1:
							if(collisionMap.size()+mapWidth>mapHeight*mapWidth) {
								throw std::string("Violated predetermined height: ")+tempLine;
							};
							break;
						}
						for(unsigned int j=0;j<mapWidth;j++){
							unsigned int temp=0;
							ss>>temp;
							if(temp==0){
								switch(phase){
								case 0:
									spriteMap[currentLayer].push_back(0);break;
								case 1:
									collisionMap.push_back(0);break;
								}
							}else
							{
								switch(phase){
								case 0:
									if((temp-1)>=tileInfos.size()) {spriteMap[currentLayer].push_back(0);}
									else spriteMap[currentLayer].push_back(temp);
									break;
								case 1:
									if(temp>=16) {collisionMap.push_back(15);}
									else collisionMap.push_back(temp);
									break;
								}
							}
							
						}
					}	
				}
			tempLine.clear();
		}
	}
	catch(std::ifstream::failure e){
		if(!map.eof()){
			map.close();
			std::cout<<"failure to load mapFile:"<<mapFile<<std::endl;
			return false;
		}
	}
	catch(std::string str){
		map.close();
		std::cout<<str<<std::endl;
		return false;
	}
	
	//fillup
	for(int i = 0 ; i < MAX_LAYERS ; i++){
		if(spriteMap[i].size()<mapWidth*mapHeight){
			for(unsigned int j=spriteMap[i].size();j<mapWidth*mapHeight;j++){
				spriteMap[i].push_back(0);
			}
		}
	}
	if(collisionMap.size()<mapWidth*mapHeight){
		for(unsigned int i=collisionMap.size();i<mapWidth*mapHeight;i++){
			collisionMap.push_back(0);
		}
	}
	map.close();
	
	return true;
}

unsigned int TileMap::getColMask(int x, int y){
	if(x>=mapWidth) x=mapWidth-1;
	if(y>=mapHeight) y=mapHeight-1;
	unsigned int found=collisionMap[(mapHeight-1-y)*mapWidth +x];
	return found;
}

unsigned int TileMap::getColMask(unsigned int tile){
	unsigned int found=collisionMap[tile];
	return found;
}