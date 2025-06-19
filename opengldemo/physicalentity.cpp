#include "physicalentity.hpp"

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes, glm::vec3 pos) : Entity(pos), mMeshes(meshes)
{

}

PhysicalEntity::PhysicalEntity(std::vector<Mesh>& meshes) : Entity(), mMeshes(meshes)
{

}

void PhysicalEntity::Draw(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_Pos);
	model = glm::scale(model, m_Scale);
	shader.UniformMatrix4f("uModel", model);

	for (auto& mesh : mMeshes)
	{
		mesh.Draw(shader);
	}
}