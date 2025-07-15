#define _CRT_SECURE_NO_WARNINGS
#include "vertex_array.hpp"
#include "vertex.hpp"
#include <iostream>
#include "glerror.hpp"

VertexArray::VertexArray(Vertex* vertices, unsigned int* indices, size_t count, size_t vertexSize, size_t indexCount) : indexCount(indexCount)
{
	std::cout << "VertexArray created!" << std::endl;
	std::cout << "Count: " << count << std::endl;
	std::cout << "Vertex size: " << vertexSize << std::endl;
	std::cout << "Index count: " << indexCount << std::endl;
	std::cout << std::endl;
	
	GLError(glGenVertexArrays(1, &VAO));
	GLError(glGenBuffers(1, &VBO));
	GLError(glGenBuffers(1, &EBO));

	GLError(glBindVertexArray(VAO));

	GLError(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	GLError(glBufferData(GL_ARRAY_BUFFER, count * vertexSize, vertices, GL_STATIC_DRAW));

	GLError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	GLError(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	GLError(glEnableVertexAttribArray(0));
	GLError(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	GLError(glEnableVertexAttribArray(1));
	GLError(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	GLError(glEnableVertexAttribArray(2));
	GLError(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));
	GLError(glEnableVertexAttribArray(3));
	GLError(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));
	GLError(glEnableVertexAttribArray(4));
	GLError(glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs)));
	GLError(glEnableVertexAttribArray(5));
	GLError(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights)));
	GLError(glEnableVertexAttribArray(6));
	GLError(glBindVertexArray(0));
}

VertexArray::~VertexArray()
{
	// std::cout << "VertexArray destroyed!" << std::endl;
	// to do later
	// glDeleteBuffers(1, &VBO);
	// glDeleteVertexArrays(1, &VAO);
	// glDeleteBuffers(1, &EBO);
}

void VertexArray::Draw() const
{
	Bind();
	GLError(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
}

void VertexArray::Bind() const
{
	// std::cout << "VertexArray binded." << std::endl;
	GLError(glBindVertexArray(VAO));
	GLError(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
}

void VertexArray::UpdateVertexData(Vertex* vertices, size_t count, size_t vertexSize)
{
	GLError(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLError(glBufferData(GL_ARRAY_BUFFER, count * vertexSize, vertices, GL_STATIC_DRAW));
}
