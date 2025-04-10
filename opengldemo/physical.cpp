#include "physical.hpp"

Physical::Physical(Shader& shader, const VertexArray& va, const Texture& texture) : Physical(glm::vec3(0.0f), glm::vec3(0.0f), shader, va, texture, {Texture("container.jpg"), Texture("container.jpg"), 32.0f})
{
}

Physical::Physical(glm::vec3 pos, glm::vec3 collisionBox, Shader& shader, const VertexArray& va, const Texture& texture, const Material& material) : RigidBody(pos, collisionBox), m_Shader(shader), m_VA(va), m_Texture(texture), m_Material(material)
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

const Material& Physical::GetMaterial() const
{
	return m_Material;
}


void Physical::Draw() const
{
	m_VA.Draw();
}






