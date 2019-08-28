#include "Physics.h"
#include "ObjBBox.h"
#include "TileMap.h"
#include "SceneManager.h"

float epsilon=0.00001f;

Physics::Physics(void) : X(0.0f), Y(0.0f), Width(0.0f), Height(0.0f), vX(0.0f), vY(0.0f),
	collisionSensors(nullptr), initted(false), movable(false), stuck(false)
{
}


Physics::~Physics(void)
{
	delete collisionSensors;
}

void Physics::setBodySize(float w,float h){
	if(!initted) return;
	TileMap* current=SceneManager::getInstance()->getCurrentMap();
	if(!current) return;
	float xIndent=(current->getMapXBounds().y - current->getMapWidth())/2;
	float yIndent=(current->getMapYBounds().y - current->getMapHeight())/2;
	float xOffset = collisionSensors->getCenter().x - X;
	float yOffset = collisionSensors->getCenter().y -Y;
	Width=w;
	Height=h;
	Width = Width < 0 ? 0 : Width>current->getMapWidth()*current->getTileWidth() ? current->getMapWidth()*current->getTileWidth() : Width;
	Height = Height < 0 ? 0 : Height>current->getMapHeight()*current->getTileHeight() ? current->getMapHeight()*current->getTileHeight() : Height;
	X = X<=xIndent + Width/2? xIndent + Width/2 
			: X>= xIndent + current->getMapWidth()*current->getTileWidth()-Width/2?
			xIndent + current->getMapWidth()*current->getTileWidth()-Width/2 
		: X;
	Y = Y<=yIndent+Height/2? yIndent+Height/2
				: Y>= yIndent + current->getMapHeight()*current->getTileHeight()-Height/2? 
				yIndent + current->getMapHeight()*current->getTileHeight() -Height/2
			: Y;
	vX=0.0f; vY=0.0f;
	collisionSensors->changeBoxPlacements(glm::vec2(X+xOffset,Y+yOffset));
	movable = false; stuck = false;
}

void Physics::setVelocity(float vx,float vy){
	if(!initted) return;
	vX=vx;
	vY=vy;
}

bool Physics::initializeCollisionBox(){
	if(!collisionSensors){
		collisionSensors = new ObjBBox;
		if(!collisionSensors) return false;
		collisionSensors->initBox(getBodyCoords(),getBodySize());
		return true;
	}
	return false;
}

//Overlook col
void Physics::setBodyCoords(float x,float y){
	if(!initted) return;
	TileMap* current=SceneManager::getInstance()->getCurrentMap();
	if(!current) return;
	float xIndent=(current->getMapXBounds().y - current->getMapWidth())/2;
	float yIndent=(current->getMapYBounds().y - current->getMapHeight())/2;
	float xOffset = collisionSensors->getCenter().x - X;
	float yOffset = collisionSensors->getCenter().y -Y;
	X=x+xIndent;
	X = X<=xIndent + Width/2? xIndent + Width/2 
			: X>= xIndent + current->getMapWidth()*current->getTileWidth()-Width/2?
			xIndent + current->getMapWidth()*current->getTileWidth()-Width/2 
		: X;
	Y=y+yIndent;
	Y = Y<=yIndent+Height/2? yIndent+Height/2
				: Y>= yIndent + current->getMapHeight()*current->getTileHeight()-Height/2? 
				yIndent + current->getMapHeight()*current->getTileHeight() -Height/2
			: Y;
	vX=0.0f; vY=0.0f;
	collisionSensors->changeBoxPlacements(glm::vec2(X+xOffset,Y+yOffset));
	movable = false; stuck = false;
}

void Physics::changeCollisionBoxAttrib(glm::vec2 newPos, glm::vec2 newSize){
	if(!initted) return;
	TileMap* current=SceneManager::getInstance()->getCurrentMap();
	if(!current) return;
	float xIndent=(current->getMapXBounds().y - current->getMapWidth())/2;
	float yIndent=(current->getMapYBounds().y - current->getMapHeight())/2;
	//Resize
	newSize.x = newSize.x <0.0f ? 0.0f : newSize.x > current->getMapWidth()*current->getTileWidth() ?
		current->getMapWidth()*current->getTileWidth()
		: newSize.x;
	newSize.y = newSize.y <0.0f ? 0.0f : newSize.y > current->getMapHeight()*current->getTileHeight() ?
		current->getMapHeight()*current->getTileHeight()
		: newSize.y;
	collisionSensors->changeBoxSize(newSize);
	float colWidthHalf=collisionSensors->getDistToEdges().x;
	float colHeightHalf=collisionSensors->getDistToEdges().y;
	//Change position
	newPos.x = newPos.x - colWidthHalf< 0.0f ? colWidthHalf + xIndent: 
		newPos.x + colWidthHalf > current->getMapWidth()*current->getTileWidth() ? xIndent +current->getMapWidth()*current->getTileWidth() - colWidthHalf
		: newPos.x + xIndent;
	newPos.y = newPos.y - colHeightHalf < 0.0f ? colHeightHalf + yIndent : 
		newPos.y + colHeightHalf > current->getMapHeight()*current->getTileHeight() ? yIndent + current->getMapHeight()*current->getTileHeight() - colHeightHalf
		: newPos.y + yIndent;
	collisionSensors->changeBoxPlacements(newPos);
	vX=0.0f; vY=0.0f;
	movable = false; stuck = false;
}

bool Physics::initPhysicsBody(glm::vec2 pos, glm::vec2 span){
	if(initted) return false;
	TileMap* current=SceneManager::getInstance()->getCurrentMap();
	if(!current) return false;
	float xIndent=(current->getMapXBounds().y - current->getMapWidth())/2;
	float yIndent=(current->getMapYBounds().y - current->getMapHeight())/2;
	Width=span.x;
	Height=span.y;
	Width = Width < 0 ? 0 : Width>current->getMapWidth()*current->getTileWidth() ? current->getMapWidth()*current->getTileWidth() : Width;
	Height = Height < 0 ? 0 : Height>current->getMapHeight()*current->getTileHeight() ? current->getMapHeight()*current->getTileHeight() : Height;
	X=pos.x + xIndent;
	X = X<=xIndent + Width/2? xIndent + Width/2 
			: X>= xIndent + current->getMapWidth()*current->getTileWidth()-Width/2?
			xIndent + current->getMapWidth()*current->getTileWidth()-Width/2 
		: X;
	Y=pos.y + yIndent;
	Y = Y<=yIndent+Height/2? yIndent+Height/2
				: Y>= yIndent + current->getMapHeight()*current->getTileHeight()-Height/2? 
				yIndent + current->getMapHeight()*current->getTileHeight() -Height/2
			: Y;
	if(!initializeCollisionBox()) return false;
	initted=true;
	return true;
}

bool Physics::updateOnMap(float delta){
	TileMap* current=SceneManager::getInstance()->getCurrentMap();
	if(!current) return false;
	if(!initted) return false;
	float xIndent=(current->getMapXBounds().y - current->getMapWidth())/2;
	float yIndent=(current->getMapYBounds().y - current->getMapHeight())/2;
	float colWidthHalf=collisionSensors->getDistToEdges().x;
	float colHeightHalf=collisionSensors->getDistToEdges().y;
	float centerX=collisionSensors->getCenter().x;
	float centerY=collisionSensors->getCenter().y;
	float offsetX = centerX - X, offsetY = centerY -Y;

//Can't update position if stucked
		if(stuck) return false;

//Check collision:
		if(movable){
			//Step X
			float newX = centerX + vX*delta;
			newX = newX<=xIndent + colWidthHalf? xIndent + colWidthHalf 
				: newX>= xIndent + current->getMapWidth()*current->getTileWidth()-colWidthHalf?
				xIndent + current->getMapWidth()*current->getTileWidth()-colWidthHalf 
			: newX;
			
			//Init top and Foot bounds
			float botY = centerY - colHeightHalf , topY = centerY + colHeightHalf; 
			unsigned int footTile = floor((botY-yIndent)/current->getTileHeight());
			unsigned int headTile = floor((topY-yIndent)/current->getTileHeight());
			unsigned int higherBounds;
			if(footTile!=headTile)
				higherBounds = topY == headTile*current->getTileHeight() + yIndent ? headTile -1 : headTile;
			else higherBounds = headTile;

			if(newX - centerX >0) 
			{
				//Left side of tile contains right of colBox
				float rightXOrigin = xIndent + floor((centerX+colWidthHalf-xIndent)/current->getTileWidth())*current->getTileWidth();
				//limit movement to max 1 tile
				if(abs(newX -centerX)>current->getTileWidth()) newX = centerX + current->getTileWidth();
				//Find tile of bottom right of colBox 
				unsigned int rightTile = floor((centerX+colWidthHalf-xIndent)/current->getTileWidth());
				if(rightTile == current->getMapWidth()) goto StepY;
				//Precheck current
				if(centerX + colWidthHalf == rightXOrigin) {
					if( rightTile == 0 ){
						X = newX - offsetX ; goto StepY;
					}
					for(unsigned int i = footTile;i<=higherBounds;i++){
						//Find tile collision status
						unsigned int currentTileCollision = current -> getColMask(rightTile, i);
						//Check if current right side is on any collision edge
						unsigned int prevTileCollision = current->getColMask(rightTile-1, i);
						if(prevTileCollision&1 || (currentTileCollision>>2)&1 ) goto StepY;
						int botOfTile = i*current->getTileHeight() + yIndent, topOfTile = botOfTile + current->getTileHeight();
						if((currentTileCollision>>1)&1 && botOfTile > (centerY-colHeightHalf) && botOfTile<(centerY+colHeightHalf)) goto StepY;
						if((currentTileCollision>>3)&1 && topOfTile > (centerY-colHeightHalf) && topOfTile<(centerY+colHeightHalf)) goto StepY;
					}
				}
				//If on last column step normally
				if(rightTile == current->getMapWidth()-1) {X = newX - offsetX ; goto StepY;}
				//Check move
				if(newX + colWidthHalf > rightXOrigin + current->getTileWidth()) {
					for(unsigned int i = footTile;i<=higherBounds;i++){	
						//Find tile collision status
						unsigned int currentTileCollision = current -> getColMask(rightTile, i);
						//Check collision:
						unsigned int nextTileCollision = current->getColMask(rightTile + 1, i);
						if((nextTileCollision >> 2 )&1 || currentTileCollision &1 ){
							X = rightXOrigin + current->getTileWidth()- colWidthHalf-offsetX;
							goto StepY;
						}
						//Check for horizontal
						int botOfTile = i*current->getTileHeight() + yIndent, topOfTile = botOfTile + current->getTileHeight();
						if(((nextTileCollision>>1)&1 && botOfTile > (centerY-colHeightHalf) && botOfTile<(centerY+colHeightHalf)) ||
							((nextTileCollision>>3)&1 && topOfTile > (centerY-colHeightHalf) && topOfTile<(centerY+colHeightHalf)) ){
								X = rightXOrigin + current->getTileWidth()- colWidthHalf-offsetX;
							goto StepY;
						}
					}
				}
				
			}
			else if(newX - centerX <0)
			{
				float leftXOrigin = xIndent + floor((centerX-colWidthHalf-xIndent)/current->getTileWidth())*current->getTileWidth();
				if(abs(newX -centerX)>current->getTileWidth()) newX = centerX - current->getTileWidth();
				unsigned int leftTile = floor((centerX-colWidthHalf-xIndent)/current->getTileWidth());
				//Step freely 
				if(leftTile == 0) {X = newX - offsetX;goto StepY;} 
				//Precheck
				if(centerX - colWidthHalf == leftXOrigin) {
					if(leftTile == current->getMapWidth()) {
						X = newX - offsetX; goto StepY;
					}
					for(unsigned int i = footTile;i<=higherBounds;i++){
						unsigned int currentTileCollision = current->getColMask(leftTile, i);
						//Check if current left side is on any collision edge
						unsigned int prevTileCollision = current->getColMask(leftTile -1, i);
						if(prevTileCollision&1 || (currentTileCollision>>2)&1 ) goto StepY;
						int botOfTile = i*current->getTileHeight() + yIndent, topOfTile = botOfTile + current->getTileHeight();
						if((currentTileCollision>>1)&1 && botOfTile > (centerY-colHeightHalf) && botOfTile<(centerY+colHeightHalf)) goto StepY;
						if((currentTileCollision>>3)&1 && topOfTile > (centerY-colHeightHalf) && topOfTile<(centerY+colHeightHalf)) goto StepY;
					}
				}
				//Check move
				if(newX - colWidthHalf < leftXOrigin) {
					for(unsigned int i = footTile;i<=higherBounds;i++){
						unsigned int currentTileCollision = current->getColMask(leftTile, i);
						//Check collision:
						unsigned int nextTileCollision = current->getColMask(leftTile - 1, i);
						if(nextTileCollision&1 || (currentTileCollision>>2) &1 ){
							X = leftXOrigin + colWidthHalf-offsetX; 
							goto StepY;
						}
						//Check for horizontal
						int botOfTile = i*current->getTileHeight() + yIndent, topOfTile = botOfTile + current->getTileHeight();
						if(((nextTileCollision>>1)&1 && botOfTile > (centerY-colHeightHalf) && botOfTile<(centerY+colHeightHalf)) ||
							((nextTileCollision>>3)&1 && topOfTile > (centerY-colHeightHalf) && topOfTile<(centerY+colHeightHalf)) ){
							X = leftXOrigin + colWidthHalf-offsetX; 
							goto StepY;
						}
					}
				}
			}
			X = newX-offsetX;

			//Step Y
			StepY:
			//Reupdate:
			collisionSensors->changeBoxPlacements(glm::vec2(X+offsetX,centerY));
			centerX=collisionSensors->getCenter().x;
			//Continue;
			float newY = centerY + vY*delta;
			newY = newY<=yIndent+colHeightHalf? yIndent+colHeightHalf 
				: newY>= yIndent + current->getMapHeight()*current->getTileHeight()-colHeightHalf? 
				yIndent + current->getMapHeight()*current->getTileHeight() -colHeightHalf
			: newY;
			
			//Init bounds for Tile checking
			float leftX = centerX - colWidthHalf, rightX = centerX + colWidthHalf;
			unsigned int leftTile = floor((leftX-xIndent)/current->getTileWidth());
			unsigned int rightTile = floor((rightX-xIndent)/current->getTileWidth());
			if(leftTile!=rightTile)
				higherBounds = rightX == rightTile*current->getTileWidth() + xIndent ? rightTile -1 : rightTile;
			else higherBounds = rightTile;

			//Left side of tile contains top of colBox
			float topYOrigin = yIndent + floor((centerY+colHeightHalf-yIndent)/current->getTileHeight())*current->getTileHeight();
			//Find tile of top of colBox 
			unsigned int topTile = floor((centerY+colHeightHalf-yIndent)/current->getTileHeight());
			float botYOrigin = yIndent + floor((centerY-colWidthHalf-yIndent)/current->getTileHeight())*current->getTileHeight();
			unsigned int botTile = floor((centerY-colHeightHalf-yIndent)/current->getTileHeight());
				
			if(newY - centerY >0) 
			{
				//limit movement to max 1 tile
				if(abs(newY -centerY)>current->getTileHeight()) newY = centerY + current->getTileHeight();
				
				if(topTile == current->getMapHeight()) goto EndUpdate;
				//Precheck
				if(centerY + colHeightHalf == topYOrigin){
					if(topTile == 0) {
						Y = newY -offsetY; goto EndUpdate;
					}
					for(unsigned int i = leftTile;i<=higherBounds;i++){
						//Find tile collision status
						unsigned int currentTileCollision = current -> getColMask(i, topTile);
						//Check if current top side is on any collision edge
						unsigned int prevTileCollision = current->getColMask(i, topTile-1);
						if((prevTileCollision>>3)&1 || (currentTileCollision>>1)&1 ) goto EndUpdate;
						int leftOfTile = i*current->getTileWidth() + xIndent, rightOfTile = leftOfTile + current->getTileWidth();
						if((currentTileCollision>>2)&1 && leftOfTile > (centerX-colWidthHalf) && leftOfTile<(centerX+colWidthHalf)) goto EndUpdate;
						if((currentTileCollision>>0)&1 && rightOfTile > (centerX-colWidthHalf) && rightOfTile<(centerX+colWidthHalf)) goto EndUpdate;
					}
				}
				//If on last row step normally
				if(topTile == current->getMapHeight()-1) {Y = newY - offsetY; goto EndUpdate;}
				//Check move
				if(newY + colHeightHalf > topYOrigin + current->getTileHeight()){
					for(unsigned int i = leftTile;i<=higherBounds;i++){
						unsigned int currentTileCollision = current -> getColMask(i, topTile);
						//Check collision:
						unsigned int nextTileCollision = current->getColMask(i, topTile+1);
						if((nextTileCollision >> 1 )&1 || (currentTileCollision>>3) &1 ){
							Y = topYOrigin + current->getTileHeight()- colHeightHalf-offsetY;
							goto EndUpdate;
						}
						//Check for vertical
						int leftOfTile = i*current->getTileWidth() + xIndent, rightOfTile = leftOfTile + current->getTileWidth();
						if(((nextTileCollision>>2)&1 && leftOfTile > (centerX-colWidthHalf) && leftOfTile<(centerX+colWidthHalf)) ||
							((nextTileCollision>>0)&1 && rightOfTile > (centerX-colWidthHalf) && rightOfTile<(centerX+colWidthHalf)) ){
								Y = topYOrigin + current->getTileHeight()- colHeightHalf-offsetY;
							goto EndUpdate;
						}
					}
				}
			}
			else if(newY - centerY <0)
			{
				if(abs(newY -centerY)>current->getTileHeight()) newY = centerY + current->getTileHeight();
				//Step freely 
				if(botTile == 0) {Y = newY -offsetY;goto EndUpdate;}

				if(centerY - colHeightHalf == botYOrigin) {
					if(botTile == current->getMapHeight()) {
						Y = newY - offsetY ; goto EndUpdate;
					}
					for(unsigned int i = leftTile;i<=higherBounds;i++){
						unsigned int currentTileCollision = current->getColMask(i, botTile);
						//Check if current left side is on any collision edge
						unsigned int prevTileCollision = current->getColMask(i, botTile-1);
						if((prevTileCollision>>3)&1 || (currentTileCollision>>1)&1 ) goto EndUpdate;
						int leftOfTile = i*current->getTileWidth() + xIndent, rightOfTile = leftOfTile + current->getTileWidth();
						if((currentTileCollision>>2)&1 && leftOfTile > (centerX-colWidthHalf) && leftOfTile<(centerX+colWidthHalf)) goto EndUpdate;
						if((currentTileCollision>>0)&1 && rightOfTile > (centerX-colWidthHalf) && rightOfTile<(centerX+colWidthHalf)) goto EndUpdate;
					}
				}
				//If not collide stepY
				if(newY - colHeightHalf < botYOrigin) {
					for(unsigned int i = leftTile;i<=higherBounds;i++){
						unsigned int currentTileCollision = current->getColMask(i, botTile);
						//Check collision:
						unsigned int nextTileCollision = current->getColMask(i, botTile-1);
						if((nextTileCollision >> 3 )&1 || (currentTileCollision>>1) &1 ){
							Y = botYOrigin + colWidthHalf-offsetY; 
							goto EndUpdate;
						}
						//Check for vertical
						int leftOfTile = i*current->getTileWidth() + xIndent, rightOfTile = leftOfTile + current->getTileWidth();
						if(((nextTileCollision>>2)&1 && leftOfTile > (centerX-colWidthHalf) && leftOfTile<(centerX+colWidthHalf)) ||
							((nextTileCollision>>0)&1 && rightOfTile > (centerX-colWidthHalf) && rightOfTile<(centerX+colWidthHalf)) ){
								Y = botYOrigin + colWidthHalf-offsetY; 
							goto EndUpdate;
						}
					}
				}
			}
			Y = newY - offsetY;
			
			EndUpdate:
			//Reupdate:
			collisionSensors->changeBoxPlacements(glm::vec2(centerX,Y+offsetY));
		}
		else
		{
			
			//No update if not on Map
			if(centerX+ colWidthHalf<xIndent ||centerX-colWidthHalf> xIndent + current->getMapWidth()*current->getTileWidth())
				return false;
			if(centerY+colHeightHalf<yIndent || centerY-colHeightHalf>yIndent + current->getMapHeight()*current->getTileHeight())
				return false;
			//Correct if colBox is in Map
			centerX = centerX<=xIndent + colWidthHalf? xIndent + colWidthHalf 
				: centerX>= xIndent + current->getMapWidth()*current->getTileWidth()-colWidthHalf?
				xIndent + current->getMapWidth()*current->getTileWidth()-colWidthHalf 
			: centerX;
			centerY = centerY<=yIndent+colHeightHalf? yIndent+colHeightHalf 
				: centerY>= yIndent + current->getMapHeight()*current->getTileHeight()-colHeightHalf? 
				yIndent + current->getMapHeight()*current->getTileHeight() -colHeightHalf
			: centerY;
			//Calculate true Y for collision
			float botY = centerY - colHeightHalf , topY = centerY + colHeightHalf; 
			float leftX = centerX - colWidthHalf, rightX = centerX + colWidthHalf;
			unsigned int leftTile = floor((leftX-xIndent)/current->getTileWidth());
			unsigned int rightTile = floor((rightX-xIndent)/current->getTileWidth());
			unsigned int footTile = floor((botY-yIndent)/current->getTileHeight());
			unsigned int headTile = floor((topY-yIndent)/current->getTileHeight());
			try{
				unsigned int higherBoundsX, higherBoundsY;
				if(leftTile!=rightTile)
					higherBoundsX = rightX == rightTile*current->getTileWidth() + xIndent ? rightTile -1 : rightTile;
				else higherBoundsX = rightTile;
				if(footTile!=headTile)
					higherBoundsY = topY == headTile*current->getTileHeight() + yIndent ? headTile -1 : headTile;
				else higherBoundsY = headTile;

				for(unsigned int i=leftTile;i<=higherBoundsX;i++){
					for(unsigned int j=footTile;j<=higherBoundsY;j++){
						if(current->getColMask(i,j) == 15) throw 1; 
						if(i != higherBoundsX){
							if((current->getColMask(i,j)>>0)&1) throw 1;
						}
						if(i != leftTile){
							if((current->getColMask(i,j)>>2)&1) throw 1;
						}
						if(j != higherBoundsY){
							if((current->getColMask(i,j)>>3)&1) throw 1;
						}
						if(j != footTile){
							if((current->getColMask(i,j)>>1)&1) throw 1;
						}
					}
				}
			}
			catch(int a){
				switch(a){
				case 1 :
					stuck = true;
					printf_s("Cannot Initialize Body's current tile because overlaps collidable edges, please reevaluate"); break;
				}
				return false;
			}
			movable = true;
		}
	return true;		
}

glm::vec2 Physics::getBottomLeft(){
	return glm::vec2(X - Width/2, Y - Height/2);
}

glm::vec2 Physics::getBodyCoords(){
	return glm::vec2(X,Y);
}

glm::vec2 Physics::getBodySize(){
	return glm::vec2(Width,Height);
}

glm::vec2 Physics::getVelocity(){
	return glm::vec2(vX,vY);
}

bool Physics::getInitState(){
	return initted;
}

bool Physics::getMovable(){
	return movable;
}