#include "ui.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

UI::UI(std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture>& textures, std::string shadername) : m_Mesh(vertices, indices, textures), m_Shader(shadername.c_str(), false), Entity()
{
	SetScale(glm::vec3(1.0f));
}

void UI::Draw()
{
	ConfigureShader();
	m_Mesh.Draw(m_Shader);
}

void UI::ConfigureShader()
{
	glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, 0.0f, 1.0f);
	m_Shader.UniformMatrix4f("uProjection", projection);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GetPos());
	model = glm::scale(model, glm::vec3(0.9f));
	m_Shader.UniformMatrix4f("uModel", model);
}

std::shared_ptr<UI> UI::NewBasic(Vertex* vertex, unsigned int* index, Texture& texture, std::string shadername)
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

	std::shared_ptr<UI> pe = std::make_shared<UI>(vertices, indices, textures, shadername);

	return pe;
}