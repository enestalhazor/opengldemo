#include "shadowmap.hpp"

ShadowMap::ShadowMap(unsigned int width, unsigned int height) : mWidth(width), mHeight(height)
{
	GLError(glGenFramebuffers(1, &mFBO));

	GLError(glGenTextures(1, &mId));
	GLError(glBindTexture(GL_TEXTURE_2D, mId));
	GLError(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLError(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
	GLError(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mId, 0));
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	Unbind();
}

void ShadowMap::Bind() const
{
	glViewport(0, 0, mWidth, mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER,  mFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
}

void ShadowMap::Unbind() const
{
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

