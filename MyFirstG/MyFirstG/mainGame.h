#pragma once
#include "gameState.h"
class ResourceManager;
class SceneManager;
class mainGame : public gameState
{
private:
	void loadSpritesAndTextures(ResourceManager*);
	void loadAniFrames(ResourceManager*);
	void loadMaps(SceneManager*);
public:
	mainGame(void);
	~mainGame(void);
	void InitContents(Application*);
	void HandleInput(Application*);
	void Update(Application*);
	void Draw(Application*);
};

