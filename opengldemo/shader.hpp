#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "material.hpp"

class Shader
{
private:

public:
	unsigned int shaderProgram;

	Shader(const char* filename, bool cubeMap);

	~Shader();

	void Uniform4f(const char* name, float f1, float f2, float f3, float f4);

	void Uniform3f(const char* name, float f1, float f2, float f3);

	void Uniform1v(const std::string name, glm::vec3 uniform);

	void Uniform1i(const char* name, int f);

	void Uniform1iv(const std::string name, const std::vector<int>& values);

	template<typename T>
	void UniformLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, const std::vector<T>& lights)
	{
		int i = 0;
		// std::cout << "UniformLight sent." << std::endl;

		for (int i = 0; i < lights.size(); i++)
		{
			Uniform1v("uLight[" + std::to_string(i) + "].ambient", ambient);
			Uniform1v("uLight[" + std::to_string(i) + "].diffuse", diffuse);
			Uniform1v("uLight[" + std::to_string(i) + "].specular", specular);
			Uniform1v("uLight[" + std::to_string(i) + "].position", lights[i].GetPos());
		}
	}

	void Uniform3fv(const char* name, int count, const float* arr);

	void Uniform1f(const char* name, float f);

	void UniformMatrix4f(const std::string& name, glm::mat4 trans);

	void Bind() const;

private:
	unsigned int CreateShader(const std::string& code, unsigned int shaderType) const;
};