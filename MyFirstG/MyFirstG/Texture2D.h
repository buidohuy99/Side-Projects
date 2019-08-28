#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
class Texture2D{
private:
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image
    // Texture configuration
    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Mag; // Filtering mode if texture pixels > screen pixels
	GLuint ID;
	// Texture image dimensions
    unsigned int Width, Height; // Width and height of loaded image in pixels
public:
	unsigned int getWidth(){return Width;};
	unsigned int getHeight() {return Height;};
	//Comparisons
	static bool compareByIDSmaller(Texture2D* a, Texture2D* b){
		return a->ID<b->ID;
	};
	static bool compareByIDDifferent(Texture2D* a, Texture2D* b){
		return a->ID!=b->ID;
	};
    // Constructor (sets default texture modes)
	Texture2D():Wrap_S(GL_CLAMP_TO_EDGE),Wrap_T(GL_CLAMP_TO_EDGE),Filter_Min(GL_NEAREST_MIPMAP_NEAREST),Filter_Mag(GL_NEAREST){
		glGenTextures(1,&ID);
	};
    // Generates texture from image data
	void Generate(unsigned int width, unsigned int height, std::string filepath, GLuint Image_Format, bool FlipVertically=false);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const{
		glBindTexture(GL_TEXTURE_2D,ID);
	}
	~Texture2D(){
		glDeleteTextures(1,&ID);
	}
};
#endif