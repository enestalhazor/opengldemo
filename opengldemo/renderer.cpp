#include "renderer.hpp"
#include "glm/gtx/string_cast.hpp"
#define GLM_ENABLE_EXPERIMENTAL

Renderer::Renderer(Camera& cam, Shader& shader) : m_Cam(cam), m_Shader(shader)
{
	std::cout << "Renderer created!" << std::endl;
	std::cout << "Renderer Camera id: " << cam.GetId() << std::endl;
	std::cout << std::endl;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	m_Shader.UniformMatrix4f("uProjection", projection);
	m_Shader.Uniform1f("uFar_plane", 25.0f);
}

void Renderer::Draw(std::vector<Light>& lights, std::unordered_map<std::string, std::shared_ptr<PhysicalEntity>>& entities)
{
	m_Shader.Bind();
	m_Shader.UniformMatrix4f("uView", m_Cam.GetViewMatrix());
	m_Shader.Uniform1v("uViewPos", m_Cam.GetPos());
	m_Shader.Uniform1i("uLightCount", lights.size());
	m_Shader.UniformLight(lights);

	std::vector<int> values;

	for (int i = 0; i < lights.size(); i++)
	{
		lights[i].GetCubeMap().BindTexture();
		values.push_back(lights[i].GetCubeMap().GetSlotNum());
	}


	m_Shader.Uniform1iv("uDepthMap", values);

	for (int i = 0; i < lights.size(); i++)
	{
		lights[i].Draw(m_Shader);
	}

	for (auto& pair: entities)
	{
		pair.second->Draw(m_Shader);
	}


}