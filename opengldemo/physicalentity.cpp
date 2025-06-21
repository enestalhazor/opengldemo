#include "physicalentity.hpp"

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes, glm::vec3 pos) : Entity(pos), m_Meshes(meshes)
{

}

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes) : Entity(), m_Meshes(meshes)
{

}

void PhysicalEntity::Draw(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_Pos);
	model = glm::scale(model, m_Scale);
	shader.UniformMatrix4f("uModel", model);

	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(shader);
	}
}

std::vector<Mesh> PhysicalEntity::GetMeshes()
{
	return m_Meshes;
}
