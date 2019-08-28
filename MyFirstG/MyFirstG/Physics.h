#pragma once

#include <glm/glm.hpp>
class ObjBBox;
class TileMap;

class Physics
{
public:
	Physics(void);
	~Physics(void);

	//Body
	bool initPhysicsBody(glm::vec2 pos, glm::vec2 span);
	void setBodyCoords(float x,float y);
	void setBodySize(float w,float h);
	void setVelocity(float vx,float vy);
	glm::vec2 getBodyCoords();
	glm::vec2 getBodySize();
	glm::vec2 getVelocity();
	bool getInitState();
	bool getMovable();
	//Utility
	glm::vec2 getBottomLeft();
	//Collision related
	void changeCollisionBoxAttrib(glm::vec2 newPos, glm::vec2 newSize);
	bool checkTileMapCollision();

	//----Main to apply phys
	bool updateOnMap(float delta);

private:
	//FLAG
	bool initted;
	bool movable, stuck;

	//Basic attribs 
	float X,Y,Width,Height;
	float vX,vY;
	//ColBox
	ObjBBox* collisionSensors;
	
	//CollisionBox
	bool initializeCollisionBox();
};

