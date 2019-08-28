#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ObjBBox
{
public:
	ObjBBox();
	~ObjBBox(void);
	
	//Functionality
	bool testAABB(const ObjBBox & other);

	//Init
	bool initBox(glm::vec2 initCenter,glm::vec2 initSpan);
	void changeBoxSize(glm::vec2 NewBoxSize);
	void changeBoxPlacements(glm::vec2 NewLocation);
	
	//Getters
	glm::vec2 getCenter() const {return centerOrigin;};
	glm::vec2 getDistToEdges() const {return distToEdges;};
	
private:
	//Get dot
	float getDot(const glm::vec2& a,const glm::vec2& b){
		return a.x*b.x+a.y*b.y;
	};

	//Attribs
	bool initted;
	glm::vec2 centerOrigin;
	glm::vec2 distToEdges; //distance from center to x-, y- bounds of box
};

