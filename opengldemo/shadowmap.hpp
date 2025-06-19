#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>
#include "glerror.hpp"

class ShadowMap
{
private:
	unsigned int mId;
	unsigned int mFBO;
	unsigned int mWidth;
	unsigned int mHeight;

public:

	ShadowMap(const unsigned int width, const unsigned int height);
	void Bind() const;
	void Unbind() const;

};