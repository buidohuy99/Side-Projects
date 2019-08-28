#pragma once
#include <glm/glm.hpp>
#include <string>

class SpriteBatch;
class Physics;
class Graphics;
class ObjBBox;

class GameObject
{
public:
	virtual void Init()=0;
	virtual void Update(float delta)=0;
	//Draw Obj
	void draw(SpriteBatch* sprBatchName);

protected:
	GameObject();
	~GameObject();
	//Initialize once when created// 
	bool InitObj(glm::vec2 position,glm::vec2 size, std::string graphicsTypeName, bool initAnim = false);

	//Get Components
	Physics* getPhysBod();
	Graphics* getGraphics();

private:
	//Status
	bool isActive;

	//Physics Component
	Physics* ph;
	//Graphics Component
	Graphics* gp;
	//Init components
	bool initializePhysicsBody(glm::vec2 pos, glm::vec2 span);
	bool initializeGraphics(std::string graphicsTypeName, bool initAnim);
};

