#define _CRT_SECURE_NO_WARNINGS
#include "shader.hpp"
#include <string>
#include "glerror.hpp"
#include "glm/gtx/string_cast.hpp"

#define GLM_ENABLE_EXPERIMENTAL

Shader::Shader(const char* filename, bool cubeMap)
{
	std::cout << "Shader created!" << std::endl;
	std::cout << "Glsl file used by the Shader: " << filename << std::endl;
	std::cout << std::endl;

	std::string vertexShaderCode;
	std::string geometryShaderCode;
	std::string fragmentShaderCode;

	FILE* f = fopen(filename, "r");
	char buffer[1000] = { 0 };
	int state = 0;

	while (fgets(buffer, 1000, f) != nullptr)
	{
		if (strcmp(buffer, "-----\n") == 0)
		{

			if (cubeMap)
			{
				if (state == 1)
				{
					state = 2;
					continue;
				}

				state = 1;
				continue;
			}
			else
			{
				state = 2;
				continue;
			}
		}

		switch (state)
		{
		case 0:
			vertexShaderCode += buffer;
			break;
		case 1:
			geometryShaderCode += buffer;
			break;
		case 2:
			fragmentShaderCode += buffer;

		default:
			break;
		}
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(f);

	unsigned int vertexShader;
	unsigned int geometryShader;
	unsigned int fragmentShader;

	vertexShader = CreateShader(vertexShaderCode, GL_VERTEX_SHADER);

	if (vertexShader == -1)
	{
		return;
	}

	if (cubeMap)
	{
		geometryShader = CreateShader(geometryShaderCode, GL_GEOMETRY_SHADER);

		if (geometryShader == -1)
		{
			return;
		}
	}

	fragmentShader = CreateShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	if (fragmentShader == -1)
	{
		return;
	}

	shaderProgram = glCreateProgram();
	GLError(glAttachShader(shaderProgram, vertexShader));
	if (cubeMap)
	{
		GLError(glAttachShader(shaderProgram, geometryShader));
	}
	GLError(glAttachShader(shaderProgram, fragmentShader));
	GLError(glLinkProgram(shaderProgram));

	int success;
	char infoLog[512];

	GLError(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
	if (!success) {
		GLError(glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog));
		std::cout << "shader program couldn't be linked\n" << infoLog << std::endl;
	}

	Bind();
	GLError(glDeleteShader(vertexShader));
	if (cubeMap)
	{
		GLError(glDeleteShader(geometryShader));
	}
	GLError(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
	std::cout << "Shader destroyed!" << std::endl;
	GLError(glDeleteProgram(shaderProgram));
}

void Shader::Bind() const
{
	//std::cout << "Shader binded" << std::endl;
	GLError(glUseProgram(shaderProgram));
}

void Shader::Uniform4f(const char* name, float f1, float f2, float f3, float f4)
{
	Bind();
	//std::cout << "Uniform4f sent." << std::endl;
	//std::cout << "Uniform4f name: " << name << std::endl;
	//std::cout << name << "values: " << f1 << f2 << f3 << f4 << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	GLError(glUniform4f(vertexColorLocation, f1, f2, f3, f4));
}

void Shader::Uniform3f(const char* name, float f1, float f2, float f3)
{

	Bind();
	//std::cout << "Uniform3f sent." << std::endl;
	//std::cout << "Uniform3f name: " << name << std::endl;
	//std::cout << name << "values: " << f1 << f2 << f3 << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	GLError(glUniform3f(vertexColorLocation, f1, f2, f3));
}

void Shader::Uniform1v(const std::string name, glm::vec3 uniform)
{
	Bind();
	//std::cout << "Uniform1v sent." << std::endl;
	//std::cout << "Uniform1v name: " << name << std::endl;
	//std::cout << name << "vector: " << glm::to_string(uniform) << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name.c_str());
	GLError(glUniform3f(vertexColorLocation, uniform.x, uniform.y, uniform.z));
}

void Shader::Uniform3fv(const char* name, int count, const float* arr)
{
	Bind();
	// std::cout << "Uniform3fv sent." << std::endl;
	// std::cout << "Uniform3fv name: " << name << std::endl;
	// std::cout << name << "count: " << count << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	GLError(glUniform3fv(vertexColorLocation, count, arr));
}

void Shader::Uniform1f(const char* name, float f)
{
	Bind();
	//std::cout << "Uniform1v sent." << std::endl;
	//std::cout << "Uniform1v name: " << name << std::endl;
	//std::cout << name << "value: " << f << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	GLError(glUniform1f(vertexColorLocation, f));
}

void Shader::Uniform1i(const char* name, int f)
{
	Bind();
	//std::cout << "Uniform1v sent." << std::endl;
	//std::cout << "Uniform1v name: " << name << std::endl;
	//std::cout << name << "value: " << f << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	GLError(glUniform1i(vertexColorLocation, f));
}

void Shader::Uniform1iv(const std::string name, const std::vector<int>& values)
{
	Bind();
	//std::cout << "Uniform1v sent." << std::endl;
	//std::cout << "Uniform1v name: " << name << std::endl;
	//std::cout << name << "value: " << f << std::endl;

	int vertexColorLocation = glGetUniformLocation(shaderProgram, name.c_str());
	GLError(glUniform1iv(vertexColorLocation, values.size(), &values[0]));
}

void Shader::UniformMatrix4f(const std::string& name, glm::mat4 trans)
{
	Bind();
	// std::cout << "UniformMatrix4f sent." << std::endl;
	// std::cout << "UniformMatrix4f name: " << name << std::endl;
	// std::cout << name << "vector: " << glm::to_string(trans) << std::endl;

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, name.c_str());
	GLError(glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans)));
}


unsigned int Shader::CreateShader(const std::string& code, unsigned int shaderType) const
{
	unsigned int shader = glCreateShader(shaderType);
	const char* str = code.c_str();
	char infoLog[512];

	int success;

	GLError(glShaderSource(shader, 1, &str, NULL));
	GLError(glCompileShader(shader));
	GLError(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		GLError(glGetShaderInfoLog(shader, 512, NULL, infoLog));
		std::cout << "shader compilation failed\n" << infoLog << std::endl;
		GLError(glDeleteShader(shader));
		return -1;
	}
	return shader;
}