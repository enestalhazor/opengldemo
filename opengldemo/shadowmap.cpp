#include "shadowmap.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ShadowMap::ShadowMap(unsigned int width, unsigned int height, float nearPlane, float farPlane) : m_Width(width), m_Height(height), m_NearPlane(nearPlane), m_FarPlane(farPlane)
{
	GLError(glGenFramebuffers(1, &m_FBO));

	GLError(glGenTextures(1, &m_Id));
	GLError(glBindTexture(GL_TEXTURE_2D, m_Id));
	GLError(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLError(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
	GLError(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Id, 0));
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	UnbindFrameBuffer();
}

void ShadowMap::BindFrameBuffer() const
{
	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER,  m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
}

void ShadowMap::UnbindFrameBuffer() const
{
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::BindTexture(unsigned int slot) const
{
	GLError(glActiveTexture(GL_TEXTURE0 + slot));
	GLError(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void ShadowMap::ConfigureShader(Shader& shader, glm::vec3 lightPos)
{
	glm::mat4 lightProjection;
	glm::mat4 lightView;

	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_NearPlane, m_FarPlane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	shader.Bind();
	shader.UniformMatrix4f("uLightSpaceMatrix", lightSpaceMatrix);
}

glm::mat4 ShadowMap::GetLightSpaceMatrix()
{
	return lightSpaceMatrix;
}

