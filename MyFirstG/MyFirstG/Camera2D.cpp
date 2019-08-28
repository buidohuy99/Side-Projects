#include "Camera2D.h"
#include <iostream>

Camera2D* Camera2D::_camera = nullptr;

Camera2D::Camera2D(void) : position(0.0f,0.0f) , orthoMatrix(1.0f), updated(false), scaler(1.0f), viewMatrix(1.0f), initted(false)
{
}


Camera2D::~Camera2D(void)
{
}

bool Camera2D::initCam(float screenWidth,float screenHeight){
	if(initted==true) return false;
	screenSizes=glm::vec2(screenWidth,screenHeight);
	position=glm::vec2(screenWidth/2,screenHeight/2);
	orthoMatrix=glm::ortho(-(screenWidth/2),screenWidth/2,-(screenHeight/2),screenHeight/2);
	initted=true;

	updated=true;
	update();

	return true;
};

bool Camera2D::update(){
	if(updated){
		viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::translate(viewMatrix,glm::vec3(-position,0.0f));
		viewMatrix = glm::scale(viewMatrix,glm::vec3(scaler,scaler,0.0f));
		updated=false;
		return true;
	};
	return false;
};

void Camera2D::moveCam(glm::vec2 moveVec){
	position += moveVec;
	updated =true;
};

void Camera2D::zoomCam(float zoomDir){
	scaler +=zoomDir;
	updated =true;
	
};
