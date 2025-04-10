#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "material.hpp"
#include "light.hpp"

class Shader
{
private:

public:
	unsigned int shaderProgram;

	Shader(const char* filename);

	~Shader();

	void Uniform4f(const char* name, float f1, float f2, float f3, float f4);

	void Uniform3f(const char* name, float f1, float f2, float f3);

	void Uniform1v(const char* name, glm::vec3 uniform);

	void Uniform1i(const char* name, int f);

	void UniformLight(Light& light);

	void Uniform3fv(const char* name, int count, const float* arr);

	void Uniform1f(const char* name, float f);

	void UniformMatrix4f(const char *name, glm::mat4 trans);

	void Bind() const;

private:
	unsigned int CreateShader(const std::string& code, unsigned int shaderType) const;
};