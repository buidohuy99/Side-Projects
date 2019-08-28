#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	void checkCompileError(GLuint shaderID,std::string type);
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  //name is name of shader uniform variable
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
	void setMatrix4(const std::string &name, glm::mat4 value) const;
	~Shader(){
		glDeleteProgram(ID);
	}
};
#endif