#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>

class Texture2D;
class Sprite;
class Shader;
class AniFrame;

class ResourceManager
{
public:
    
	static ResourceManager* createInstance(){
		if(!resourceManager){
			resourceManager= new ResourceManager;
		}
		return resourceManager;
	};
   static Shader* LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name);
   static Shader* GetShader(std::string name);
   static Texture2D* LoadTexture(const GLchar *file, GLboolean alpha, std::string name,
	   unsigned int width, unsigned int height);
   static Texture2D* GetTexture(std::string name);
   static Sprite* LoadSprite(std::string rectsFile,std::string texName, std::string name);
   static Sprite* GetSprite(std::string name);
   static AniFrame* LoadAniFrame(std::string name, std::string spriteName, std::vector<unsigned int> spriteSTTVec);
   static AniFrame* GetAniFrame(std::string name);
   static void Clear();
private:
	// Resource storage
    static std::map<std::string, Shader*>    Shaders;
    static std::map<std::string, Texture2D*> Textures;
	static std::map<std::string, Sprite*> Sprites;
	static std::map<std::string, AniFrame*> AniFrames;
	static ResourceManager* resourceManager;
	ResourceManager() { }
    static Shader*    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
	static Sprite* loadSpriteFromFile(std::string rectsFile,std::string texT);
    static Texture2D* loadTextureFromFile(const GLchar *file, GLboolean alpha, unsigned int width, unsigned int height);
	~ResourceManager() { }
};

#endif