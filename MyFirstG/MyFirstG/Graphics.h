#pragma once
#include <string>
#include <glm/glm.hpp>

class Sprite;
struct Rect;
class Animator;
struct ColorRGBA8;
class Texture2D;
class SpriteBatch;

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	//Init
	bool initGraphics(std::string graphicsTypeName, bool initAnim);

	//if use Sprite
	void setCurrentSubSprite(unsigned int sprSTT);
	void setNewSprite(std::string spriteName);
	
	//if use Animator
	void step();
	void jump(unsigned int jlen);

	//Drawing options
	void setObjRotate(float angle);
	void setObjScale(glm::vec2 scale);
	void setDepth(float deep);
	void setFlip(bool flipping){
		flip=flipping;
	}

	//Draw
	void draw(SpriteBatch* sprBatchName, Rect* destRect);

	//Get
	Rect* getTexRect();
	Texture2D* getTex();

private:
	bool useAnimator;
	//Sprite
	Sprite* objSprite;
	Rect* currentSubSprite;
	//Or Animator
	Animator* objAnimator;
	
	//Draw thingz
	float rotateFactor, deepness;
	bool flip;
	glm::vec2 scaleFactor;

	static ColorRGBA8 generalCol;
	//Init functions
	bool initializeBaseSprite(std::string spriteName);
	bool initializeAnimator(std::string aniFrameName);
};

