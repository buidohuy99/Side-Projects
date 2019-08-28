#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	void Init();
	void Update(float delta);
	Player(void);
	~Player(void);
	void setUp();
	void setDown();
	void setLeft();
	void setRight();
private:
	bool up,down,left,right;
	bool moving;
	void UpdatePhysics(float delta);
};

