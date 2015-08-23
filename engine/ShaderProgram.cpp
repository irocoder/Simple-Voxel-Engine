#include "ShaderProgram.hpp"
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vShaderPath, const std::string& fShaderPath)
{
	readShader(&vertexShaderSource, vShaderPath);
	readShader(&fragmentShaderSource, fShaderPath);
	//fragmentShaderSource = XL::readTXT(fShaderPath);

	vertexSource = vertexShaderSource.c_str(); //does the memory actually get copied over?  recheck this!
	fragmentSource = fragmentShaderSource.c_str();
	//DOUBLE RECHECK MEMORY ALLOCATION OF FRAG_SOURCE AND VERT_SOURCE

	loadShaders();
	bind();
}

void ShaderProgram::readShader(std::string* shader, const std::string& filePath)
{
	std::ifstream input(filePath);

	if (!input)
	{
		std::cout << "Filepath : " << filePath << " not valid" << std::endl;
		//return NULL;
	}

	std::string s;

	while (std::getline(input, s))
	{
		shader->append(s);
		shader->append("\n");
	}

	for(int i = 0; i < shader->size(); i++)
	{
		std::cout << shader->at(i);
	}

	//input.close();
}

void ShaderProgram::bind()
{
	glUseProgram(program);
}

void ShaderProgram::unbind()
{
	glUseProgram(NULL);
}

void ShaderProgram::loadShaders()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//glBindFragDataLocation(program, 0, "outputColor");
	glLinkProgram(program);
}

ShaderProgram::~ShaderProgram()
{
	std::cout << "DELETING SHADER-PROGRAM!" << std::endl;
	glLinkProgram(NULL);
	glUseProgram(NULL);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}