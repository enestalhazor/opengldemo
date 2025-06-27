#pragma once

#include "cubemap.hpp"
#include "physicalentity.hpp"
#include "mesh.hpp"

class Light : public PhysicalEntity
{
private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	glm::vec3 m_Pos;
	CubeMap m_CubeMap;
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 pos, std::vector<Mesh>& meshes);
	glm::vec3& GetAmbient();
	glm::vec3& GetDiffuse();
	glm::vec3& GetSpecular();
	CubeMap& GetCubeMap();


};