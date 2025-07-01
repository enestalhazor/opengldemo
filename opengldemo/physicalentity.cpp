#include "physicalentity.hpp"

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes, glm::vec3 pos) : Entity(pos), m_Meshes(meshes)
{

}

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes) : Entity(), m_Meshes(meshes)
{

}

void PhysicalEntity::Draw(Shader& shader)
{
	shader.UniformMatrix4f("uModel", GetModelMatrix());

	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(shader);
	}
}