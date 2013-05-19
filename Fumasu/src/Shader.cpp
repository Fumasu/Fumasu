/*
 * Shader.cpp
 *
 *  Created on: 10.03.2012
 *      Author: Fumasu
 */

#include "Fumasu/Shader.hpp"

namespace Fumasu
{

GLuint Shader::getHandle() {
	return programHandle;
}

Shader::Shader(std::string vertex, std::string fragment, bool fromFile)
{
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	if(vertexShaderHandle != 0)
	{
		std::string shaderCode;
		if(fromFile) shaderCode = LoadFromFile(vertex);
		else shaderCode =vertex;
		const char *shader =shaderCode.c_str();

		glShaderSource(vertexShaderHandle, 1, &shader, NULL);
		glCompileShader(vertexShaderHandle);
	}

	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	if(fragmentShaderHandle != 0)
	{
		std::string shaderCode;
		if(fromFile) shaderCode =LoadFromFile(fragment);
		else shaderCode =fragment;
		const char *shader = shaderCode.c_str();

		glShaderSource(fragmentShaderHandle, 1, &shader, NULL);
		glCompileShader(fragmentShaderHandle);
	}

	programHandle = glCreateProgram();
	if(programHandle != 0)
	{
		glAttachShader(programHandle, vertexShaderHandle);
		glAttachShader(programHandle, fragmentShaderHandle);

		glLinkProgram(programHandle);
	}
}

void Shader::Uniform(std::string location, int value)
{
	glUniform1i(glGetUniformLocation(programHandle, location.c_str()), value);
}

void Shader::Use()
{
	glUseProgram(programHandle);
}

std::string Shader::getErrorLog()
{
	char buf[1000];
	int len;
	std::string content;

	glGetShaderInfoLog(fragmentShaderHandle, 1000, &len, buf);
	content = buf;

	glGetShaderInfoLog(vertexShaderHandle, 1000, &len, buf);
	content += buf;

	glGetProgramInfoLog(programHandle, 1000, &len, buf);
	content += buf;

	return content;
}

char* Shader::LoadFromFile(std::string filename)
{
	FILE *fd;
	char *str;
	long len, r;

	fd = fopen(filename.c_str(), "r");

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	str = (char*) malloc(len * sizeof(char));

	r = fread(str, sizeof(char), len, fd);

	str[r - 1] = '\0';

	fclose(fd);

	//Logger::getInstance()->print(str);

	return str;
}

Shader::~Shader() {
	glDetachShader(programHandle, fragmentShaderHandle);
	glDetachShader(programHandle, vertexShaderHandle);

	glDeleteShader(fragmentShaderHandle);
	glDeleteShader(vertexShaderHandle);

	glDeleteProgram(programHandle);
}

}
/* namespace Fumasu */
