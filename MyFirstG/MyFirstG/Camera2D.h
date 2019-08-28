#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:
	~Camera2D(void);

	static Camera2D* instance(){
		if(_camera==nullptr){
			_camera=new Camera2D;
		};
		return _camera;
	};

	//Cam functions
	bool initCam(float screenWidth,float screenHeight);
	bool update();
	void moveCam(glm::vec2 moveVec);
	void zoomCam(float zoomDir);

	//setters
	void setPosition(glm::vec2 pos) { position=pos; updated=true;};
	void setScaler(float scale) { scaler=scale; updated=true;};

	//getters
	glm::vec2 getPosition() { return position/scaler; };
	glm::vec2 getScreenSizes() { return screenSizes/scaler;};
	float getScaler() { return scaler; };
	bool getUpdatedStatus() {return updated;};
	glm::mat4 getViewMatrix() {return viewMatrix;};
	glm::mat4 getOrthoMatrix() {return orthoMatrix;};

private:
	Camera2D(void);
	static Camera2D* _camera;
	glm::vec2 position;
	glm::vec2 screenSizes;
	glm::mat4 orthoMatrix;
	glm::mat4 viewMatrix;
	float scaler;

	bool updated;
	bool initted;
};

