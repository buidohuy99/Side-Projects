#include "Animator.h"
#include "ResourceManager.h"
#include "Sprite.h"

Animator::Animator(void):frameSeq(nullptr)
{
	current=0;
}

Animator::~Animator(void)
{
}

AniFrame::AniFrame():spriteForAni(nullptr)
{

}

unsigned int AniFrame::getSequenceSize(){
	return sequence.size();
}

AniFrame::~AniFrame(){
	
}

bool Animator::aniFrameInit(){
	return frameSeq==nullptr?false:true;
}

bool AniFrame::InitSprite(std::string spriteName){
	if(spriteForAni) return false;
	spriteForAni=ResourceManager::createInstance()->GetSprite(spriteName);
	if(spriteForAni) return true;
	return false;
}

bool AniFrame::addNewState(unsigned int stateSTT){
	if(!spriteForAni) return false;
	Rect* temp=spriteForAni->findSubSprite(stateSTT);
	if(!temp) return false;
	sequence.emplace_back(stateSTT);
	return true;
}

bool AniFrame::removeState(unsigned int statePos){
	if(!spriteForAni||sequence.size()==0) return false;
	if(statePos>=sequence.size()) return false;
	sequence.erase(sequence.begin()+statePos);
	return true;
}

bool AniFrame::isEmpty(){
	return sequence.size()==0;
}

bool AniFrame::isInitted(){
	return spriteForAni==nullptr?false:true;
}

Rect* AniFrame::getState(unsigned int statePos)
{
	if(statePos>=sequence.size()) return nullptr;
	return spriteForAni->findSubSprite(sequence[statePos]);
}

Texture2D* AniFrame::getTexture(){
	if(spriteForAni==nullptr) return nullptr;
	return spriteForAni->getTex();
}

bool Animator::setAniFrame(std::string aniFrame){
	if(frameSeq) return false;
	auto found=ResourceManager::createInstance()->GetAniFrame(aniFrame);
	if(found==nullptr) return false;
	frameSeq=found;
	return true;
}

bool Animator::step(){
	if(!frameSeq->isInitted()||frameSeq->isEmpty()) return false;
	current++;
	current=current%frameSeq->getSequenceSize();
	return true;
}

bool Animator::jump(unsigned int jump){
	if(!frameSeq->isInitted()||frameSeq->isEmpty()) return false;
	current+=jump;
	current=current%frameSeq->getSequenceSize();
	return true;
}

Rect* Animator::getTexRect(){
	if(!frameSeq->isInitted()||frameSeq->isEmpty()) return false;
	return frameSeq->getState(current);
}

Texture2D* Animator::getTex(){
	if(!frameSeq->isInitted()||frameSeq->isEmpty()) return false;
	return frameSeq->getTexture();
}