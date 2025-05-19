#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vertex.hpp"


class VertexArray
{
private:
	size_t indexCount;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

public:

	VertexArray(Vertex* vertices, unsigned int* indices, size_t count, size_t vertexSize, size_t indexCount);

	~VertexArray();

	void Draw() const;
	void Bind() const;
};