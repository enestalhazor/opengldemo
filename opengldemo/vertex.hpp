#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexArray
{
private:
	int indexCount;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

public:

	VertexArray(float* vertices, unsigned int* indices, int count, int vertexSize, int indexCount);

	~VertexArray();

	void Draw() const;
};