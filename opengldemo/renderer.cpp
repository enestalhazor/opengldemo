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

	s.UniformMatrix4f("view", m_Cam.GetViewMatrix());
	s.UniformMatrix4f("projection", projection);

	s.UniformMatrix4f("model", model);
	s.Uniform4f("uColor", color.x, color.y, color.z, color.w);

	v.Draw();
}

void Renderer::Render(Physical& obj)
{
	obj.BindVA();
	obj.BindShader();
	obj.BindTexture();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

	obj.GetShader().UniformMatrix4f("view", m_Cam.GetViewMatrix());
	obj.GetShader().UniformMatrix4f("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(obj.getPos()));
	model = glm::rotate(model, glm::radians(obj.GetYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(obj.GetPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(obj.GetScale()));

	obj.GetShader().UniformMatrix4f("model", model);
	obj.GetShader().Uniform4f("uColor", obj.GetColor().x, obj.GetColor().y, obj.GetColor().z, 1.0f);

	obj.Draw();
}

void Renderer::RenderUI(Physical& obj)
{
	obj.BindVA();
	obj.BindShader();
	obj.BindTexture();

	obj.GetShader().UniformMatrix4f("view", glm::mat4(1.0f));
	obj.GetShader().UniformMatrix4f("projection", glm::mat4(1.0f));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(obj.getPos()));
	model = glm::rotate(model, glm::radians(obj.GetYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(obj.GetPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(obj.GetScale()));

	obj.GetShader().UniformMatrix4f("model", model);
	obj.GetShader().Uniform4f("uColor", obj.GetColor().x, obj.GetColor().y, obj.GetColor().z, 1.0f);

	obj.Draw();
}
