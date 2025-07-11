#pragma once
#include "entity.hpp"
#include "mesh.hpp"
#include "model.hpp"

class PhysicalEntity : public Entity
{
private:
	std::vector<Mesh> m_Meshes;
	
public:

	PhysicalEntity(std::vector<Mesh> meshes, glm::vec3 pos);
	PhysicalEntity(std::vector<Mesh> meshes);
	void Draw(Shader& shader);

	// factory function for creating a basic PhysicalEntity object
	static std::shared_ptr<PhysicalEntity> NewBasic(Vertex* vertex, unsigned int* i, Texture& textures);
	// factory function for creating a PhysicalEntity object from model
	static std::shared_ptr<PhysicalEntity> NewFromModel(const Model &model);

};