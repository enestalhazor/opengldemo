#pragma once

#include "cubemap.hpp"
#include "physicalentity.hpp"
#include "mesh.hpp"

class Light : public PhysicalEntity
{
private:
	CubeMap m_CubeMap;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, std::vector<Mesh> meshes);
	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;
	CubeMap& GetCubeMap();

};