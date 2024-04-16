#include"Shader.h"

Shader::Shader()
{
	isAvailable = false;
	attributeIndex = 0;
	id = 0;
	vertexShaderPath = "";
	fragmentShaderPath = "";
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	vertexShaderPath = vertexPath;
	fragmentShaderPath = fragmentPath;
	std::string vertexCode = ReadShaderFile(vertexPath);
	std::string fragmentCode = ReadShaderFile(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	ProcessShaderType(vertex, vShaderCode, VERTEXSHADER, infoLog);
	ProcessShaderType(fragment, fShaderCode, FRAGMENTSHADER, infoLog);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
void Shader::use() {

	glUseProgram(id);

}
bool Shader::ShaderAvailable() {
	return isAvailable;
}
std::string Shader::ReadShaderFile(const char* path) {
	std::string code;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return code;
}
bool Shader::ProcessShaderType(GLuint& id, const char* shaderSource, Shader::ShaderType shaderType, char* infoLog)
{
	memset(infoLog, 0, 512);
	id = glCreateShader(shaderType);
	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		PrintShaderCompileInfo(id, shaderType, infoLog);
	}
	return success;
}
void Shader::PrintShaderCompileInfo(GLuint id, Shader::ShaderType shaderType, char* infoLog)
{
	glGetShaderInfoLog(id, 512, NULL, infoLog);
	std::string shaderTypeStr = shaderType == VERTEXSHADER ? "VERTEX" : "FRAGMENT";
	std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
}
void Shader::ProcessShaderProgram(GLuint& id, GLuint vertex, GLuint fragment, char* infoLog)
{
	memset(infoLog, 0, 512);
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return;
	}
	isAvailable = true;
}
Shader::~Shader() {
	glDeleteProgram(id);
}

