#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <unordered_map>
#include "light.hpp"

class Renderer
{
private:
	Camera& m_Cam;
	Shader& m_Shader;
public:
	Renderer(Camera& cam, Shader& shader);

	void Draw(std::vector<Light>& lights, std::unordered_map<std::string ,std::shared_ptr<PhysicalEntity>>& entities);

};