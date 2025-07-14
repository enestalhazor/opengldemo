#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>

class Texture
{
private:
	std::string m_Type;
	std::string m_Path;
	unsigned int m_Id;
	unsigned int FBO;

public:
	
	Texture(std::string path, std::string directory, std::string type, bool flip);

	void Bind() const;
	void Bind(unsigned int slot) const;

	std::string GetPath();
	std::string GetType();
	unsigned int GetFBO();
	unsigned int GetId();


};