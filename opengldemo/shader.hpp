#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	unsigned int shaderProgram;

public:

	Shader(const char* filename);

	~Shader();

	void Uniform4f(const char* name, float f1, float f2, float f3, float f4);

	void Uniform1f(const char* name, float f);

	void UniformMatrix4f(const char *name, glm::mat4 trans);

private:
	unsigned int CreateShader(const std::string& code, unsigned int shaderType) const;
};