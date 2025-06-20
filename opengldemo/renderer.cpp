#include "renderer.hpp"
#include "glm/gtx/string_cast.hpp"
#define GLM_ENABLE_EXPERIMENTAL


Renderer::Renderer(const Camera& cam) : m_Cam(cam)
{
	std::cout << "Renderer created!" << std::endl;
	std::cout << "Renderer Camera id: " << cam.GetId() << std::endl;
	std::cout << std::endl;
}

void Renderer::Render(const VertexArray& v, Shader& s, const Texture& t, const glm::mat4& model, const glm::vec4& color)
{
	std::cout << "VertexArray, Shader, Texture binded." << std::endl;
	std::cout << "Renderer Uniform mat4 uModel: " << glm::to_string(model) << std::endl;
	std::cout << "Renderer Uniform vec3 uColor: " << glm::to_string(color) << std::endl;

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
	std::cout << "VertexArray, Shader, Texture binded." << std::endl;
	std::cout << "Renderer Uniform mat4 uModel: " << glm::to_string(model) << std::endl;
	std::cout << "Renderer Uniform vec3 uColor: " << glm::to_string(color) << std::endl;
	std::cout << "Renderer Uniform vec3 uLightColor: " << glm::to_string(lightColor) << std::endl;
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