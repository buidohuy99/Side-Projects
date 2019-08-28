#include "Shader.h"

void Shader::checkCompileError(GLuint shaderID,std::string type){
	GLint success=0;
	if(type!="PROGRAM"){
		glGetShaderiv(shaderID,GL_COMPILE_STATUS,&success);
		if(!success){
			char Log[512];
			glGetShaderInfoLog(shaderID,512,NULL,Log);
			std::cout<<Log<<std::endl;
		}
	}else
	{
		glGetProgramiv(shaderID,GL_LINK_STATUS,&success);
		if(!success){
			char Log[512];
			glGetProgramInfoLog(shaderID,512,NULL,Log);
			std::cout<<Log<<std::endl;
		}
		
	}
}

// constructor reads and builds the shader
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
	std::string vertexString,fragmentString;
	std::ifstream vertexFile,fragmentFile;
	// ensure ifstream objects can throw exceptions:
    vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try 
    {	
		//open
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		//read content to stream
		std::stringstream vertexStream,fragmentStream;
		vertexStream<<vertexFile.rdbuf();
		fragmentStream<<fragmentFile.rdbuf();
		//close
		vertexFile.close();
		fragmentFile.close();
		//stream to string
		vertexString=vertexStream.str();
		fragmentString=fragmentStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
	
	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();
	
	//Compiling Shader Program
	//Vertex Shader
	unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexSource,NULL);
	glCompileShader(vertexShader);
	checkCompileError(vertexShader,"SHADER");
	
	//Fragment Shader
	unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentSource,NULL);
	glCompileShader(fragmentShader);
	checkCompileError(fragmentShader,"SHADER");
	
	//Shader Program
	ID=glCreateProgram();
	glAttachShader(ID,vertexShader);
	glAttachShader(ID,fragmentShader);
	glLinkProgram(ID);
	checkCompileError(ID,"PROGRAM");
	
	//Delete shaders after making program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
}

// use/activate the shader
void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
} //name is name of shader uniform variable

void Shader::setInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const std::string &name, float value) const{
	glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setMatrix4(const std::string &name, glm::mat4 value) const{
	glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

