#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>
#include "glerror.hpp"
#include "shader.hpp"

class CubeMap
{
private:
	unsigned int mId;
	unsigned int mFBO;
	unsigned int mWidth;
	unsigned int mHeight;
	float mNearPlane;
	float mFarPlane;

public:
	CubeMap(unsigned int width, unsigned height, float nearPlane, float farPlane);
	void BindFrameBuffer() const;
	void BindTexture(unsigned int slot) const;
	void UnbindFrameBuffer() const; 

	// this method prepares the given shader for creating cube map
	void ConfigureShader(Shader& shader, glm::vec3 lightPos) const;
};