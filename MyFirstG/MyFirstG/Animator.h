#pragma once
#include <vector>
class Sprite;
struct Rect; 
class Texture2D;

class AniFrame{
public:
	AniFrame();
	~AniFrame();
	bool InitSprite(std::string spriteName);
	bool addNewState(unsigned int stateSTT);
	bool removeState(unsigned int statePos);
	Rect* getState(unsigned int statePos);
	Texture2D* getTexture();
	bool isEmpty();
	bool isInitted();
	unsigned int getSequenceSize();
private:
	Sprite* spriteForAni;
	std::vector<unsigned int> sequence;
};

class Animator
{
public:
	Animator(void);
	~Animator(void);
	
	bool setAniFrame(std::string aniFrame);
	bool step();
	bool jump(unsigned int jump);
	bool aniFrameInit();
	Rect* getTexRect();
	Texture2D* getTex();
	
private:
	AniFrame* frameSeq;
	unsigned int current;
};