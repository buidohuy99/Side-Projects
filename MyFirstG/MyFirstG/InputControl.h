#pragma once

//From Benjamin Arnold

#include <unordered_map>
#include <glm/glm.hpp>

class InputControl
{
public:
	static InputControl* getInstance(){
		if(IControl==nullptr){
			IControl=new InputControl;
		}
		return IControl;
	}
	~InputControl(void);

	void update();

    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);

    void setMouseCoords(float x, float y);

    /// Returns true if the key is held down
    bool isKeyDown(unsigned int keyID);

    /// Returns true if the key was just pressed
    bool isKeyPressed(unsigned int keyID);

    //getters
    glm::vec2 getMouseCoords() const { return _mouseCoords; }
private:
	 /// Returns true if the key is held down
    bool wasKeyDown(unsigned int keyID);

	InputControl(void);
	static InputControl* IControl;
	std::unordered_map<unsigned int, bool> keyMap;
	std::unordered_map<unsigned int, bool> prevKeyMap;

	glm::vec2 _mouseCoords;
};

