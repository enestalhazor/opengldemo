#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "vertex.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "physical.hpp"

class Renderer
{
private:
	const Camera& m_Cam;
public:
	Renderer(const Camera& cam);

	void Render(const VertexArray& v, Shader& s, const Texture& t, const glm::mat4& model, const glm::vec4& color);

	void Render(const VertexArray& v, Shader& s, glm::mat4 model, glm::vec3 color, glm::vec3 lightColor);

	void Render(Physical& obj);

	void RenderUI(Physical& obj);
};