#include "Window.h"
#include <vector>
class Window;
class ResourceManager;
class SceneManager;
class InputControl;
class gameState;

class Application{
	
private:
	Application();

	//App wise
	static Application* myApplication;
	Window* window;
	bool isActive;
	std::vector<gameState*> statesVec;
	gameState* currentState;
	unsigned int currentID;

	//Game scene Wise , Load when game loads
	ResourceManager* resourceManager;
	SceneManager* sceneManager;
	InputControl* inputControl;

	//FPS WISE
	float fps;
	float physicsfps;
	void calculateFPS();
	
	//Get input for all 
	SDL_Event inputChecker;
	void SDLEvtHandling(SDL_Event& e);
	
	//Load game states
	void loadAllGameStates();

	//Will use when app scene system is implimented
	void InitContents();
	void HandleInput();
	void Update();
	void Draw();

public:
	~Application();
	static Application* createInstance();

	//Functionality related
	void Run();
	bool addState(gameState* state);
	bool changeState(unsigned int stateNum);
	unsigned int getCurrentStateID();

	//Getters
	SceneManager* getSceneManager();
	ResourceManager* getResourceManager();
	InputControl* getInputControl();
};
