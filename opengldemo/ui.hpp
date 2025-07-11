#pragma once
#include "mesh.hpp"
#include "entity.hpp"

class UI : public Entity
{
private:
	Mesh m_Mesh;
	Shader m_Shader;

public:

	UI(std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture>& textures, std::string shadername);
	void Draw();
	void ConfigureShader();
	static std::shared_ptr<UI> NewBasic(Vertex* vertex, unsigned int* i, Texture& textures, std::string shadername);

};