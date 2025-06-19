#include "cubemap.hpp"
#include <vector>

CubeMap::CubeMap(unsigned int width, unsigned int height, float nearPlane, float farPlane) : mWidth(width), mHeight(height), mNearPlane(nearPlane), mFarPlane(farPlane)
{
	glGenFramebuffers(1, &mFBO);
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mId);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	UnbindFrameBuffer();
}

void CubeMap::BindFrameBuffer() const
{
	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void CubeMap::BindTexture(unsigned int slot) const
{
	GLError(glActiveTexture(GL_TEXTURE0 + slot));
	GLError(glBindTexture(GL_TEXTURE_CUBE_MAP, mId));
}


void CubeMap::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeMap::ConfigureShader(Shader& shader, glm::vec3 lightPos) const
{
	glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), (float)mWidth / (float)mHeight, mNearPlane, mFarPlane);

	std::vector<glm::mat4> shadowTransforms;

	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	shader.Bind();

	for (unsigned int i = 0; i < 6; ++i)
	{
		shader.UniformMatrix4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	shader.Uniform1f("far_plane", mFarPlane);
	shader.Uniform1v("lightPos", lightPos);
}