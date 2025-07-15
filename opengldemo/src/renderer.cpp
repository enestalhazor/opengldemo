#include "renderer.hpp"
#include "glm/gtx/string_cast.hpp"
#define GLM_ENABLE_EXPERIMENTAL

Renderer::Renderer(Camera& cam, Shader& shader) : m_Cam(cam), m_Shader(shader), m_ScreenW(800), m_ScreenH(600)
{
	std::cout << "Renderer created!" << std::endl;
	std::cout << "Renderer Camera id: " << cam.GetId() << std::endl;
	std::cout << std::endl;


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.001f, 100.0f);

	m_Shader.UniformMatrix4f("uProjection", projection);
	m_Shader.Uniform1f("uFar_plane", 25.0f);
}

void Renderer::Render(std::vector<Light>& lights, std::unordered_map<std::string, std::shared_ptr<PhysicalEntity>>& entities, Shader& shader)
{
	// 1.
	for (int i = 0; i < lights.size(); i++)
	{
		lights[i].GetCubeMap().ConfigureShader(shader, lights[i].GetPos());
		lights[i].GetCubeMap().BindFrameBuffer();

		for (auto& pair : entities) {
			pair.second->Draw(shader);
		}

		lights[i].GetCubeMap().UnbindFrameBuffer();
	}

	// 2.
	glViewport(0, 0, m_ScreenW, m_ScreenH);
	glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	for (auto& pair : entities)
	{
		pair.second->Draw(m_Shader);
	}
}

void Renderer::RenderUI(std::unordered_map<std::string, std::shared_ptr<UIEntity>>& UIs)
{
	glViewport(0, 0, m_ScreenW, m_ScreenH);

	for (auto& pair : UIs)
	{
		pair.second->Draw();
	}
}

unsigned int Renderer::GetScreenW()
{
	return m_ScreenW;
}

unsigned int Renderer::GetScreenH()
{
	return m_ScreenH;
}

void Renderer::SetScreenW(int w)
{
	m_ScreenW = w;
}

void Renderer::SetScreenH(int h)
{
	m_ScreenH = h;
}