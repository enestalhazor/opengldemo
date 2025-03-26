#define _CRT_SECURE_NO_WARNINGS
#include "shader.hpp"


Shader::Shader(const char* filename)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	FILE* f = fopen(filename, "r");
	char buffer[1000] = { 0 };
	int state = 0;

	while (fgets(buffer, 1000, f) != nullptr)
	{
		if (strcmp(buffer, "-----\n") == 0)
		{
			state = 1;
			continue;
		}

		switch (state)
		{
		case 0:
			vertexShaderCode += buffer;
			break;
		case 1:
			fragmentShaderCode += buffer;
			break;


		default:
			break;
		}
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(f);


	unsigned int fragmentShader;
	unsigned int vertexShader;

	vertexShader = CreateShader(vertexShaderCode, GL_VERTEX_SHADER);

	if (vertexShader == -1)
	{
		return;
	}

	fragmentShader = CreateShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	if (fragmentShader == -1)
	{
		return;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "shader program couldn't be linked\n" << infoLog << std::endl;
	}

	Bind();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::Bind() const
{
	glUseProgram(shaderProgram);
}

void Shader::Uniform4f(const char* name, float f1, float f2, float f3, float f4)
{
	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	glUniform4f(vertexColorLocation, f1, f2, f3, f4);
}

void Shader::Uniform1f(const char* name, float f)
{
	int vertexColorLocation = glGetUniformLocation(shaderProgram, name);
	glUniform1f(vertexColorLocation, f);
}

void Shader::UniformMatrix4f(const char* name, glm::mat4 trans)
{
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

unsigned int Shader::CreateShader(const std::string& code, unsigned int shaderType) const
{
	unsigned int shader = glCreateShader(shaderType);
	const char* str = code.c_str();
	char infoLog[512];

	int success;

	glShaderSource(shader, 1, &str, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "shader compilation failed\n" << infoLog << std::endl;
		glDeleteShader(shader);
		return -1;
	}
	return shader;
}