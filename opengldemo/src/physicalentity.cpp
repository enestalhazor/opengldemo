#include "physicalentity.hpp"

PhysicalEntity::PhysicalEntity(std::vector<Mesh> meshes, glm::vec3 pos) : Entity(pos), m_Meshes(meshes)
{

}

PhysicalEntity::PhysicalEntity(std::vector<Mesh> meshes) : Entity(), m_Meshes(meshes)
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

std::shared_ptr<PhysicalEntity> PhysicalEntity::NewBasic(Vertex* vertex, unsigned int* index, Texture& texture)
{
	std::vector<unsigned int> indices;
	for (int i = 0; i < 6; i++)
	{
		indices.emplace_back(index[i]);
	}

	std::vector<Vertex> vertices;
	for (int i = 0; i < 4; i++)
	{
		vertices.emplace_back(vertex[i]);
	}

	std::vector<Texture> textures;
	textures.emplace_back(texture);

	std::vector<Mesh> meshes;
	Mesh mesh(vertices, indices, textures);
	meshes.emplace_back(mesh);

	std::shared_ptr<PhysicalEntity> pe = std::make_shared<PhysicalEntity>(meshes);

	return pe;
}

std::shared_ptr<PhysicalEntity> PhysicalEntity::NewFromModel(const Model& model)
{
	std::shared_ptr<PhysicalEntity> pe = std::make_shared<PhysicalEntity>(model.m_Meshes);
	return pe;
}