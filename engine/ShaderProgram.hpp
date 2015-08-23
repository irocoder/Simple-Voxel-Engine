#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <GL/glew.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const std::string& vShaderPath, const std::string& fShaderPath);
	~ShaderProgram();

	void bind();
	void unbind();

	GLuint get() { return program; }

private:
	void readShader(std::string* shader, const std::string& filePath);
	void loadShaders();

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	const char* vertexSource;
	const char* fragmentSource;

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint program;

	GLint positionAttribute;
	GLint colorAttribute;
};

#endif