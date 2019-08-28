#include "Texture2D.h"

void Texture2D::Generate(unsigned int width, unsigned int height, std::string filepath, GLuint Image_Format, bool flipVertically){
	int w,h,channels;
	this->Image_Format=Image_Format;
	Internal_Format=Image_Format;
	int desired = (Image_Format==GL_RGBA)? 4 : 3;
	glBindTexture(GL_TEXTURE_2D,ID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,Wrap_S);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,Wrap_T);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,Filter_Min);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,Filter_Mag);
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* imgLoad=stbi_load(filepath.c_str(),&w,&h,&channels,desired);
	Width=width; Height=height;
	if(imgLoad){
		glTexImage2D(GL_TEXTURE_2D,0,Internal_Format,width,height,0,this->Image_Format,GL_UNSIGNED_BYTE,imgLoad);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(imgLoad);
	// Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}