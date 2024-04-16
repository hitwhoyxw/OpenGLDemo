#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <GL/glew.h>    
#define GLEW_STATIC 
#include<stdlib.h>
class Shader
{
public:
	enum ShaderType
	{
		VERTEXSHADER = GL_VERTEX_SHADER,
		FRAGMENTSHADER = GL_FRAGMENT_SHADER,
	};
public:
	
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	std::string ReadShaderFile(const char* path);
	bool ProcessShaderType(GLuint& id, const char* shaderSource, Shader::ShaderType shaderType, char* infoLog);
	void PrintShaderCompileInfo(GLuint id, Shader::ShaderType shaderType, char* infoLog);
	void ProcessShaderProgram(GLuint& id, GLuint vertex, GLuint fragment, char* infoLog);
	bool ShaderAvailable();
	~Shader();
private:
	GLuint id;
	GLuint attributeIndex;
	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	bool isAvailable = false;
};

