#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.hpp>
#include "glerror.hpp"
#include "shader.hpp"
#include "light.hpp"

class ShadowMap
{
private:
	unsigned int m_Id;
	unsigned int m_FBO;
	unsigned int m_Width;
	unsigned int m_Height;
	float m_NearPlane;
	float m_FarPlane;
	glm::mat4 lightSpaceMatrix;

public:

	ShadowMap(const unsigned int width, const unsigned int height, float nearPlane, float farPlane);
	void BindFrameBuffer() const;
	void UnbindFrameBuffer() const;
	void BindTexture(unsigned int slot) const;
	void ConfigureShader(Shader& shader, Light& light);
	glm::mat4 GetLightSpaceMatrix();

};