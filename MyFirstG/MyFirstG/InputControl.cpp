#include "InputControl.h"

InputControl* InputControl::IControl=nullptr;

InputControl::InputControl(void) : _mouseCoords(0.0f)
{
}


InputControl::~InputControl(void)
{
}

void InputControl::update() {
    // Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
    for (auto& it : keyMap) {
        prevKeyMap[it.first] = it.second;
    }
}

void InputControl::pressKey(unsigned int keyID) {
    // Here we are treating _keyMap as an associative array.
    // if keyID doesn't already exist in _keyMap, it will get added
    keyMap[keyID] = true;
}

void InputControl::releaseKey(unsigned int keyID) {
    keyMap[keyID] = false;
}

void InputControl::setMouseCoords(float x, float y) {
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

bool InputControl::isKeyDown(unsigned int keyID) {
    // We dont want to use the associative array approach here
    // because we don't want to create a key if it doesnt exist.
    // So we do it manually
    auto it = keyMap.find(keyID);
    if (it != keyMap.end()) {
        // Found the key
        return it->second;
    } else {
        // Didn't find the key
        return false;
    }
}

bool InputControl::isKeyPressed(unsigned int keyID) {
    // Check if it is pressed this frame, and wasn't pressed last frame
    if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
        return true;
    }
    return false;
}

bool InputControl::wasKeyDown(unsigned int keyID) {
    // We dont want to use the associative array approach here
    // because we don't want to create a key if it doesnt exist.
    // So we do it manually
    auto it = prevKeyMap.find(keyID);
    if (it != prevKeyMap.end()) {
        // Found the key
        return it->second;
    } else {
        // Didn't find the key
        return false;
    }
}
