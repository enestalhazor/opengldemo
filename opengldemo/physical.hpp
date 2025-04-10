#pragma once
#include <glm/glm.hpp>
#include "vertex.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "rigid_body.hpp"
#include "material.hpp"

class Physical : public RigidBody
{
protected:
	Shader& m_Shader;
	const VertexArray& m_VA;
	const Texture& m_Texture;
	glm::vec3 m_Color;
	Material m_Material;

public:
	Physical(Shader& shader, const VertexArray& va, const Texture& texture);
	Physical(glm::vec3 pos, glm::vec3 collisionBox, Shader& shader, const VertexArray& va, const Texture& texture, const Material& material);
	void BindShader() const;
	void BindVA() const;
	void BindTexture() const;
	void Draw() const;
	Shader& GetShader();
	const Material& GetMaterial() const;

};

