#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>
#include "glerror.hpp"
#include "shader.hpp"
#include "light.hpp"

class CubeMap
{
private:
	unsigned int m_Id;
	unsigned int m_FBO;
	unsigned int m_Width;
	unsigned int m_Height;
	float m_NearPlane;
	float m_FarPlane;

public:
	CubeMap(unsigned int width, unsigned height, float nearPlane, float farPlane);
	void BindFrameBuffer() const;
	void BindTexture(unsigned int slot) const;
	void UnbindFrameBuffer() const;

	// this method prepares the given shader for creating cube map
	void ConfigureShader(Shader& shader, Light& light) const;
};