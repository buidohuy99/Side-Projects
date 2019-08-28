#include "ObjBBox.h"

ObjBBox::ObjBBox(void)
{
	initted=false;
	centerOrigin=glm::vec2(0.0f,0.0f);
	distToEdges=glm::vec2(0.0f,0.0f);
}

bool ObjBBox::initBox(glm::vec2 initCenter,glm::vec2 initSpan){
	if(initted!=false) return false;
	initted=true;
	centerOrigin=initCenter;
	distToEdges=glm::vec2(initSpan.x/2,initSpan.y/2);
	return true;
}

void ObjBBox::changeBoxSize(glm::vec2 NewBoxSize){
	if(initted==false) return;
	distToEdges=glm::vec2(NewBoxSize.x/2,NewBoxSize.y/2);
};

void ObjBBox::changeBoxPlacements(glm::vec2 NewLocation){
	if(initted==false) return;
	centerOrigin=NewLocation;
}

ObjBBox::~ObjBBox(void)
{
}

bool ObjBBox::testAABB(const ObjBBox & other){
	if(initted!=false){
		if(abs(centerOrigin[0]-other.centerOrigin[0]) > abs(distToEdges[0]+other.distToEdges[0])) return 0;
		if(abs(centerOrigin[1]-other.centerOrigin[1]) > abs(distToEdges[1]+other.distToEdges[1])) return 0;
		return 1;
	}
	return 0;
}