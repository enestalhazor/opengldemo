#include "cubemap.hpp"
#include <vector>

CubeMap::CubeMap(unsigned int width, unsigned int height, float nearPlane, float farPlane) : m_Width(width), m_Height(height), m_NearPlane(nearPlane), m_FarPlane(farPlane)
{
	glGenFramebuffers(1, &m_FBO);
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_Id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	UnbindFrameBuffer();
}

void CubeMap::BindFrameBuffer() const
{
	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void CubeMap::BindTexture(unsigned int slot) const
{
	GLError(glActiveTexture(GL_TEXTURE0 + slot));
	GLError(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id));
}


void CubeMap::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeMap::ConfigureShader(Shader& shader, Light& light) const
{
	glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), (float)m_Width / (float)m_Height, m_NearPlane, m_FarPlane);

	std::vector<glm::mat4> shadowTransforms;

	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(light.GetPos(), light.GetPos() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	shader.Bind();

	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.UniformMatrix4f("uShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	shader.Uniform1f("uFar_plane", m_FarPlane);

	shader.Uniform1v("uLightPos", light.GetPos());
}