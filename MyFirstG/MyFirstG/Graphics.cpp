#include "Graphics.h"
#include "Vertex.h"
#include "Sprite.h"
#include "Rect.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "SpriteBatch.h"

ColorRGBA8 Graphics::generalCol = ColorRGBA8(255,255,255,255);

Graphics::Graphics(void) : currentSubSprite(nullptr),
	objSprite(nullptr),
	deepness(0),
	scaleFactor(glm::vec2(1.0f,1.0f)),
	rotateFactor(0.0f),
	flip(false), useAnimator(false), objAnimator(nullptr)
{
}


Graphics::~Graphics(void)
{
}

bool Graphics::initGraphics(std::string graphicsTypeName, bool initAnim){
	if(!initAnim){
		if(!initializeBaseSprite(graphicsTypeName)) return false;
	}else
	{
		if(!initializeAnimator(graphicsTypeName)) return false;
	}
	return true;
}

bool Graphics::initializeBaseSprite(std::string spriteName){
	if(objSprite==nullptr&&!useAnimator){
		Sprite* temp=ResourceManager::GetSprite(spriteName);
		objSprite=temp;
		return true;
	};
	return false;
}

bool Graphics::initializeAnimator(std::string aniFrameName){
	if(!objAnimator){
		objAnimator=new Animator;
		if(!objAnimator) return false;
		useAnimator=true;
	}
	if(!objAnimator->aniFrameInit()){
		if(!objAnimator->setAniFrame(aniFrameName)) return false;
		return true;
	}
	return false;
}

void Graphics::setNewSprite(std::string spriteName){
	if(objSprite!=nullptr&&!useAnimator){
		Sprite* temp=ResourceManager::GetSprite(spriteName);
		objSprite=temp;
	};
}

void Graphics::setObjRotate(float angle){
	rotateFactor=angle;
}

void Graphics::setObjScale(glm::vec2 scale){
	scaleFactor=scale;
}

void Graphics::setCurrentSubSprite(unsigned int sprSTT){
	if(objSprite== nullptr || useAnimator) return;
	Rect* found = objSprite->findSubSprite(sprSTT);
	currentSubSprite = found;
}

void Graphics::setDepth(float deep){
	deepness=deep;
}

void Graphics::draw(SpriteBatch* sprBatchName, Rect* destRect){
	if(useAnimator){
		if(objAnimator->aniFrameInit()){
			sprBatchName->draw(*destRect,*objAnimator->getTexRect(),flip,objAnimator->getTex(),deepness,generalCol,rotateFactor,scaleFactor);
		}
	}else
	{
		if (objSprite!= nullptr && currentSubSprite!= nullptr) {
			sprBatchName->draw(*destRect,*currentSubSprite,flip,objSprite->getTex(),deepness,generalCol,rotateFactor,scaleFactor);
		}
	}
}