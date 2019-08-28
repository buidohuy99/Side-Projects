#include "Player.h"
#include "SceneManager.h"
#include "TileMap.h"
#include "InputControl.h"
#include "Graphics.h"
#include "Physics.h"

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 32
Player::Player(void) : up(false),down(false),left(false),right(false),moving(false)
{
}


Player::~Player(void)
{
}

void Player::Init(){
	TileMap* current = SceneManager::getInstance()->getCurrentMap();
	if(!current) return;
	glm::vec2 centerOfPlayer = current->getSpawnSpot() + glm::vec2(CHAR_WIDTH/2,CHAR_HEIGHT/2);
	InitObj(centerOfPlayer,glm::vec2(CHAR_WIDTH,CHAR_HEIGHT),"CharWalk",true);
}

void Player::UpdatePhysics(float delta){
	if(getPhysBod()->getMovable()){
		//If not decided to move return
		if(up==down && down==left && left==right && !right) return;
		float veloX = 0.0f, veloY = 0.0f;
		//Change speed pixel/s to pixel/ms
		float moveMent = 2.0f/1000.0f;
		//Check diagonalsss
		if((up||down)&&(right||left)&&(up!=down)&&(right!=left)) moveMent = sqrt(moveMent/2);
		if(up) veloY = veloY + moveMent;
		if(down) veloY = veloY - moveMent;
		if(left) veloX = veloX - moveMent;
		if(right) veloX = veloX + moveMent;
		//If not move in any dir return
		if(veloX == 0.0f && veloY == 0.0f) return;
		//If move set velo and update
		glm::vec2 currentVelo = getPhysBod()->getVelocity();
		if(currentVelo.x != veloX || currentVelo.y != veloY) 
			getPhysBod()->setVelocity(veloX,veloY);
	}
	getPhysBod()->updateOnMap(delta);
	up = down = left = right = false;
}

void Player::Update(float delta){
	UpdatePhysics(delta);
	getGraphics()->setDepth(getPhysBod()->getBottomLeft().y);
}

void Player::setUp(){
	up = true;
}

void Player::setDown(){
	down = true;
}

void Player::setLeft(){
	left = true;
}
	
void Player::setRight(){
	right = true;
}