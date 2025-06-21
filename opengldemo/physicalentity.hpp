#pragma once
#include "entity.hpp"
#include "mesh.hpp"

class PhysicalEntity : public Entity
{
private:
	std::vector<Mesh>& m_Meshes;
	
public:

	PhysicalEntity(std::vector<Mesh>& meshes, glm::vec3 pos);
	PhysicalEntity(std::vector<Mesh>& meshes);
	void Draw(Shader& shader);
	std::vector<Mesh> GetMeshes();
};