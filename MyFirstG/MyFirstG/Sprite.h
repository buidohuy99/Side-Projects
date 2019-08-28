#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <map>
struct Rect;
class Texture2D;
class Sprite
{
public:
	Sprite();
	~Sprite();
	//Texcoords written in not normalized format
	bool initSprite(std::string textureName, const std::vector<glm::vec4>& texCoords);
	bool changeUVCoords(const glm::vec4& texCoords, unsigned int STT);
	Rect* findSubSprite(unsigned int sprSTT);
	Texture2D* getTex() {return spTex;}
private:
	Texture2D* spTex;
	std::vector<Rect> textureSamples;
};

