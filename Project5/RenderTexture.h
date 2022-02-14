#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "Mesh.h"

class RenderTexture
{
private:
	Texture m_texture;
	GLuint m_FBO;
	GLenum m_readWrite;
	GLenum m_format;
	GLsizei m_width;
	GLsizei m_height;

public:

	void SetFBO(GLuint FBO)
	{
		m_FBO = FBO;
	}

	void GenerateTexture(GLsizei width, GLsizei height, GLenum internalFormat, GLenum readWrite)
	{
		m_width = width;
		m_height = height;
		m_format = internalFormat;
		m_readWrite = readWrite;
		const GLfloat texBorder[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glGenTextures(1, &m_texture.texID);
		glBindTexture(GL_TEXTURE_2D, m_texture.texID);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, texBorder);
	}

	void GenerateDepthTexture(GLsizei width, GLsizei height, GLenum internalFormat, GLenum readWrite)
	{
		m_width = width;
		m_height = height;
		m_format = internalFormat;
		m_readWrite = readWrite;
		const GLfloat texBorder[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glGenTextures(1, &m_texture.texID);
		glBindTexture(GL_TEXTURE_2D, m_texture.texID);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, texBorder);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	}

	GLuint GetFBO()
	{
		return m_FBO;
	}

	GLuint GetTextureID()
	{
		return m_texture.texID;
	}

	GLenum GetReadWrite()
	{
		return m_readWrite;
	}

	GLenum GetFormat()
	{
		return m_format;
	}

	GLsizei GetWidth()
	{
		return m_width;
	}

	GLsizei GetHeight()
	{
		return m_height;
	}
};

#endif