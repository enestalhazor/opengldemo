#include "physical.hpp"

Physical::Physical(Shader& shader, const VertexArray& va, const Texture& texture, glm::vec3 color) : RigidBody(), m_Shader(shader), m_VA(va), m_Texture(texture), m_Color(color)
{
}

void Physical::BindShader() const
{
	m_Shader.Bind();
}

void Physical::BindVA() const
{
	m_VA.Bind();
}

void Physical::BindTexture() const
{
	m_Texture.Bind();
}

Shader& Physical::GetShader()
{
	return m_Shader;
}

glm::vec3 Physical::GetColor() const
{
	return m_Color;
}

void Physical::Draw() const
{
	m_VA.Draw();
}






