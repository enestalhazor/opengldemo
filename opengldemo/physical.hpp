#pragma once
#include <glm/glm.hpp>
#include "vertex.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "rigid_body.hpp"

class Physical : public RigidBody
{
protected:
	Shader& m_Shader;
	const VertexArray& m_VA;
	const Texture& m_Texture;
	glm::vec3 m_Color;

public:

	Physical(Shader& shader, const VertexArray& va, const Texture& m_Texture, glm::vec3 color);
	void BindShader() const;
	void BindVA() const;
	void BindTexture() const;
	void Draw() const;
	Shader& GetShader();
	glm::vec3 GetColor() const;
};

