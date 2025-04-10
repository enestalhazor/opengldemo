#include "renderer.hpp"

Renderer::Renderer(const Camera& cam) : m_Cam(cam)
{
}

void Renderer::Render(const VertexArray& v, Shader& s, const Texture& t, const glm::mat4& model, const glm::vec4& color)
{
	v.Bind();
	s.Bind();
	t.Bind();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

	s.UniformMatrix4f("uView", m_Cam.GetViewMatrix());
	s.UniformMatrix4f("uProjection", projection);

	s.UniformMatrix4f("uModel", model);
	s.Uniform4f("uColor", color.x, color.y, color.z, color.w);

	v.Draw();
}

void Renderer::Render(const VertexArray& v, Shader& s, glm::mat4 model, glm::vec3 color, glm::vec3 lightColor)
{
	v.Bind();
	s.Bind();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

	s.UniformMatrix4f("uView", m_Cam.GetViewMatrix());
	s.UniformMatrix4f("uProjection", projection);

	s.UniformMatrix4f("uModel", model);
	s.Uniform3f("uObjectColor", color.x, color.y, color.z);
	s.Uniform3f("uLightColor", lightColor.x, lightColor.y, lightColor.z);

	v.Draw();
}


void Renderer::Render(Physical& obj)
{
	obj.BindVA();
	obj.BindShader();
	obj.BindTexture();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

	obj.GetShader().UniformMatrix4f("uView", m_Cam.GetViewMatrix());
	obj.GetShader().UniformMatrix4f("uProjection", projection);
	obj.GetShader().Uniform1f("material.shininess", 32.0f);
	obj.GetShader().Uniform1i("material.diffuse", 0);
	obj.GetShader().Uniform1i("material.specular", 1);

	obj.GetMaterial().diffuseMap.Bind(0);
	obj.GetMaterial().specularMap.Bind(1);

	glm::mat4 model = obj.GetModelMatrix();

	obj.GetShader().UniformMatrix4f("uModel", model);
	obj.Draw();
}

void Renderer::RenderUI(Physical& obj)
{
	obj.BindVA();
	obj.BindShader();
	obj.BindTexture();

	obj.GetShader().UniformMatrix4f("uView", glm::mat4(1.0f));
	obj.GetShader().UniformMatrix4f("uProjection", glm::mat4(1.0f));

	glm::mat4 model = obj.GetModelMatrix();

	obj.GetShader().UniformMatrix4f("uModel", model);
	obj.Draw();
}
