/*
 * Shader.hpp
 *
 *  Created on: 10.03.2012
 *      Author: Fumasu
 */

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <GL/glew.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "Fumasu/Logger.hpp"

namespace Fumasu
{

class Shader
{
public:
	Shader(std::string vertex, std::string fragment, bool fromFile =true);
	~Shader();

	void Use();
	std::string getErrorLog();
	void Uniform(std::string location, int value);
	GLuint getHandle();
private:
	unsigned int fragmentShaderHandle;
	unsigned int vertexShaderHandle;
	unsigned int programHandle;
	char *vertexShader;
	char *fragmentShader;

	char *LoadFromFile(std::string filename);
};

} /* namespace Fumasu */
#endif /* SHADER_HPP_ */
