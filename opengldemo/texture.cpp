#include "texture.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glerror.hpp"

Texture::Texture(std::string path, std::string directory, std::string type) : m_Path(path), m_Type(type)
{
	//std::cout << "Texture created!" << std::endl;
	//std::cout << "Texture path: " << directory << "/" << path << std::endl;
	//std::cout << "Texture type: " << m_Type << std::endl;
	//std::cout << std::endl;

	std::string filename = directory + '/' + path;

	GLError(glGenTextures(1, &m_Id));

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLError(glBindTexture(GL_TEXTURE_2D, m_Id));
		GLError(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLError(glGenerateMipmap(GL_TEXTURE_2D));

		GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
	}

}

void Texture::Bind() const
{
	// std::cout << "Texture binded." << std::endl;
	Bind(0);
}

void Texture::Bind(unsigned int slot) const
{
	GLError(glActiveTexture(GL_TEXTURE0 + slot));
	GLError(glBindTexture(GL_TEXTURE_2D, m_Id));
}

std::string Texture::GetPath()
{
	return m_Path;
}

std::string Texture::GetType()
{
	// std::cout << std::endl;

	return m_Type;
}

unsigned int Texture::GetFBO()
{
	return FBO;
}

unsigned int Texture::GetId()
{
	return m_Id;
}



