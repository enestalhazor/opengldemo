#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "glerror.hpp"

class Mesh
{
private:
	std::vector<Vertex>       m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture>      m_Textures;
	VertexArray m_Va;

public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : m_Vertices(vertices), m_Indices(indices), m_Textures(textures),
		m_Va(&vertices[0], &indices[0], vertices.size(), sizeof(Vertex), indices.size())
	{
	}

	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			std::string number;
			std::string name = m_Textures[i].GetType();

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			shader.Uniform1i(("uMaterial." + name + number).c_str(), i);
			m_Textures[i].Bind(i);
		}

		m_Va.Draw();
		GLError(glActiveTexture(GL_TEXTURE0));
	}

};