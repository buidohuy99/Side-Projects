#include "GameObject.h"
#include "SpriteBatch.h"
#include "Physics.h"
#include "Graphics.h"
#include "Rect.h"
#include "ObjBBox.h"

GameObject::GameObject(void) : gp(nullptr), ph(nullptr),isActive(false)
{
	
}


GameObject::~GameObject(void)
{
	delete gp;
	delete ph;
}

bool GameObject::InitObj(glm::vec2 position,glm::vec2 size, std::string graphicsTypeName, bool initAnim){
	if(isActive==true) return false;
	if(!initializePhysicsBody(position,size)) return false;
	if(!initializeGraphics(graphicsTypeName,initAnim)) return false;
	isActive=true;
	return true;
}

bool GameObject::initializePhysicsBody(glm::vec2 pos, glm::vec2 span){
	if(!ph) {
		ph=new Physics;
		if(!ph) return false;
	}
	if(!ph->initPhysicsBody(pos,span)) return false;
	return true;
}

bool GameObject::initializeGraphics(std::string graphicsTypeName, bool initAnim ){
	if(!gp){
		gp=new Graphics;
		if(!gp) return false;
	}
	if(!gp->initGraphics(graphicsTypeName,initAnim)) return false;
	return true;
}

void GameObject::draw(SpriteBatch* sprBatchName){
	if(isActive==false||!ph||!gp) return;
	glm::vec2 bottomLeft=ph->getBottomLeft();
	Rect temp;
	glm::vec2 span=ph->getBodySize();
	temp.setPosition(bottomLeft.x,bottomLeft.y);
	temp.setSpan(span.x,span.y);
	gp->draw(sprBatchName,&temp);
}

Physics* GameObject::getPhysBod(){
	return ph;
}

Graphics* GameObject::getGraphics(){
	return gp;
}
