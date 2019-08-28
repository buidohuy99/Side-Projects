#pragma once
#include <string>
class Application;

class gameState
{
public:
	gameState(void);
	~gameState(void);
	virtual void InitContents(Application*) = 0;
	virtual void HandleInput(Application*) = 0;
	virtual void Update(Application*) = 0;
	virtual void Draw(Application*) = 0;

protected:
	//Change state
	bool changeState(std::string State);
};

