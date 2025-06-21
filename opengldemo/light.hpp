#pragma once
#include "mesh.hpp"
#include "physicalentity.hpp"
#include "shader.hpp"
#include <vector>

class Light : public Entity
{
private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos);
	glm::vec3 GetAmbient();
	glm::vec3 GetDiffuse();
	glm::vec3 GetSpecular();
};