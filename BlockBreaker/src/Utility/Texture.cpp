#include "Texture.h"
#include "GLEW/glew.h"
#include "stb/stb_image.h"

Texture::Texture()
	:m_RendererID(0), m_Height(0), m_Width(0), m_InternalFormat(GL_RGB),
	m_ImageFormat(GL_RGB), m_Wrap_S(GL_CLAMP_TO_EDGE), m_Wrap_T(GL_CLAMP_TO_EDGE),
	m_FilterMax(GL_LINEAR), m_FilterMin(GL_LINEAR) {}

Texture::Texture(int width, int height)
	:m_RendererID(0), m_Height(height), m_Width(width), m_InternalFormat(GL_RGB),
	 m_ImageFormat(GL_RGB), m_Wrap_S(GL_CLAMP_TO_EDGE), m_Wrap_T(GL_CLAMP_TO_EDGE), 
	 m_FilterMax(GL_LINEAR), m_FilterMin(GL_LINEAR){}

void Texture::Clean()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Generate(unsigned char* data)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_ImageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(0, m_RendererID);
}
