#include "Rect.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Texture2D.h"

Sprite::Sprite(void)
{
	spTex=NULL;
}


Sprite::~Sprite(void)
{
}

bool Sprite::initSprite(std::string textureName, const std::vector<glm::vec4>& texCoords){
	Texture2D* temp = ResourceManager::GetTexture(textureName);
	if( temp ) {
		spTex=temp;
		textureSamples.resize(texCoords.size());
		textureSamples.reserve(texCoords.size());
		for(unsigned int i=0;i<texCoords.size();i++){
			Rect temp;
			temp.setPosition(texCoords[i].x/spTex->getWidth(),(spTex->getHeight()-texCoords[i].y-texCoords[i].w)/spTex->getHeight());
			temp.setSpan(texCoords[i].z/spTex->getWidth(),texCoords[i].w/spTex->getHeight());
			textureSamples[i] = temp;
		}
		return true;
	}
	return false;
}

bool Sprite::changeUVCoords(const glm::vec4& texCoords, unsigned int STT){
	if( spTex && STT<textureSamples.size()) {
		textureSamples[STT].setPosition(texCoords.x/spTex->getWidth(),(spTex->getHeight()-texCoords.y-texCoords.w)/spTex->getHeight());
		textureSamples[STT].setSpan(texCoords.z/spTex->getWidth(),texCoords.w/spTex->getHeight());
		return true;
	}
	return false;
}

Rect* Sprite::findSubSprite(unsigned int sprSTT){
	if( spTex && sprSTT<textureSamples.size()) {
		return &(textureSamples[sprSTT]);
	}
	return nullptr;
}

