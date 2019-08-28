#include "ResourceManager.h"
#include "Texture2D.h"
#include "Sprite.h"
#include "Shader.h"
#include "Animator.h"
#include <string>

// Instantiate static variables
std::map<std::string, Texture2D*>    ResourceManager::Textures;
std::map<std::string, Shader*>       ResourceManager::Shaders;
std::map<std::string, Sprite*> ResourceManager::Sprites;
std::map<std::string, AniFrame*> ResourceManager::AniFrames;
ResourceManager* ResourceManager::resourceManager=nullptr;

Shader* ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name){
	Shader* temp = loadShaderFromFile(vShaderFile,fShaderFile);
	std::pair<std::string,Shader*> newPair(name,temp);
	Shaders.insert(newPair);
	return newPair.second;
}
Shader* ResourceManager::GetShader(std::string name){
	auto found=Shaders.find(name);
	if(found!=Shaders.end())
		return found->second;
	return nullptr;
}

Texture2D* ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name, 
										unsigned int width, unsigned int height){
	Texture2D* temp = loadTextureFromFile(file,alpha,width,height);
	std::pair<std::string,Texture2D*> newPair(name,temp);
	Textures.insert(newPair);
	return newPair.second;
}

Texture2D* ResourceManager::GetTexture(std::string name){
	auto found=Textures.find(name);
	if(found!=Textures.end())
		return found->second;
	return nullptr;
}

AniFrame* ResourceManager::LoadAniFrame(std::string name, std::string spriteName, std::vector<unsigned int> spriteSTTVec){
	auto found= AniFrames.find(name);
	if(found!=AniFrames.end()) return found->second;
	AniFrame* temp= new AniFrame;
	if(temp==nullptr) return temp;
	temp->InitSprite(spriteName);
	if(!temp->isInitted()) {
		delete temp;
		return nullptr;
	}
	for(auto it= spriteSTTVec.begin();it!=spriteSTTVec.end();it++){
		unsigned int tempNum=(*it);
		if(!temp->addNewState(tempNum)){
			temp->addNewState(0);
		}; 
	}
	std::pair<std::string,AniFrame*> newPair(name,temp);
	AniFrames.insert(newPair);
	return newPair.second; 
}

AniFrame* ResourceManager::GetAniFrame(std::string name){
	auto found=AniFrames.find(name);
	if(found!=AniFrames.end())
		return found->second;
	return nullptr;
}

Shader* ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile){
	Shader* temp= new Shader(vShaderFile,fShaderFile);
	return temp;
}

Texture2D* ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha,unsigned int width, unsigned int height){
	Texture2D* temp= new Texture2D;
	GLuint imgFormat= (alpha==GL_TRUE) ? GL_RGBA : GL_RGB;
	temp->Generate(width, height, file,imgFormat,true);
	return temp;
}

Sprite* ResourceManager::LoadSprite(std::string rectsFile, std::string texName, std::string name){
	//Sprite already same name
	Sprite* temp=GetSprite(name);
	if(temp) return temp;
	//find Extension spr
	size_t lastdot=rectsFile.rfind('.',rectsFile.length());
	if(lastdot==std::string::npos) return nullptr;
	if((rectsFile.substr(lastdot+1,rectsFile.length()-1)).compare("spr")!=0) return nullptr;

	temp = loadSpriteFromFile(rectsFile,texName);
	if (temp==nullptr) return nullptr;
	std::pair<std::string,Sprite*> newPair(name,temp);
	Sprites.insert(newPair);
	return newPair.second;
}

Sprite* ResourceManager::GetSprite(std::string name){
	auto found=Sprites.find(name);
	if(found!=Sprites.end())
		return found->second;
	return nullptr;
}

Sprite* ResourceManager::loadSpriteFromFile(std::string rectsFile,std::string texT){
	Sprite* temp = new Sprite;
	//Stuffs
	std::ifstream file;
	std::vector<glm::vec4> subSpriteRects;
	//Check ok
	file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
	try{
		file.open(rectsFile);
		std::string tempStr;
		int time=-1;
		int n=0;
		while(std::getline(file,tempStr)){
			if(tempStr.empty()) continue;
			if(time>=0){
				if (time>=n) break;
				std::stringstream ss(tempStr);
				//Temps 
				unsigned int f1,f2,f3,f4;
				std::string tempSubSpriteName;
				ss>>f1>>f2>>f3>>f4;
				subSpriteRects[unsigned int(time)]=glm::vec4(float(f1),float(f2),float(f3),float(f4));
			}else
			{
				std::stringstream ss(tempStr);
				ss>>n;
				subSpriteRects.resize(n);
				subSpriteRects.reserve(n);
			}
			time++;
			tempStr.clear();
		}
	}
	catch(std::ifstream::failure e){
		if(!file.eof()){
			file.close();
			std::cout<<"Fail to load file"<<std::endl;
			return nullptr;
		}
	}
	file.close();
	if(!temp->initSprite(texT,subSpriteRects)) return nullptr;
	return temp;
}

void ResourceManager::Clear(){
	// (Properly) delete all 
    for (std::map<std::string,AniFrame*>::iterator iter=AniFrames.begin();iter!=AniFrames.end();iter++){
		iter->second->~AniFrame();
		delete iter->second;
	}
	// (Properly) delete all sprites	
	for (std::map<std::string,Sprite*>::iterator iter=Sprites.begin();iter!=Sprites.end();iter++){
		iter->second->~Sprite();
		delete iter->second;
	}
	// (Properly) delete all textures
    for (std::map<std::string,Texture2D*>::iterator iter=Textures.begin();iter!=Textures.end();iter++){
		iter->second->~Texture2D();
		delete iter->second;
	}
	// (Properly) delete all shaders	
	for (std::map<std::string,Shader*>::iterator iter=Shaders.begin();iter!=Shaders.end();iter++){
		iter->second->~Shader();
		delete iter->second;
	}
}