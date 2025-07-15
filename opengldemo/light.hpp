#pragma once

#include "cubemap.hpp"
#include "entity.hpp"
#include "mesh.hpp"

class Light : public Entity
{
private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	glm::vec3 m_Pos;
	CubeMap m_CubeMap;
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos);
	glm::vec3& GetAmbient();
	glm::vec3& GetDiffuse();
	glm::vec3& GetSpecular();
	void SetAmbient(const glm::vec3& ambient);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetSpecular(const glm::vec3& specular);
	CubeMap& GetCubeMap();


};