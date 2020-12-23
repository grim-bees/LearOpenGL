#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	ID = glCreateProgram();	
	int vertex = add(vertexPath, GL_VERTEX_SHADER);
	int fragment = add(fragmentPath, GL_FRAGMENT_SHADER);
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	int success;
	char infoLog[250];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR:;SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

GLuint Shader::add(const char* path, int shaderType)
{
	std::map<int, std::string> SHADER_TYPE_LABEL = { {GL_VERTEX_SHADER, "VERTEX"}, {GL_FRAGMENT_SHADER, "FRAGMENT"} };
	std::string shaderCode;	
	std::ifstream shaderFile;	
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);	
	try
	{
		shaderFile.open(path);		
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();		
		shaderFile.close();
		shaderCode = shaderStream.str();		
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* code = shaderCode.c_str();
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::" << SHADER_TYPE_LABEL[shaderType] << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	return shader;	
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
