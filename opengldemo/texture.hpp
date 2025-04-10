#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>

class Texture
{
private:
	std::string filename;
	unsigned int texture;
	int nrChannels;
	int height;
	int width;
public:
	Texture(const char* filename);
	void Bind() const;
	void Bind(unsigned int slot) const;

};