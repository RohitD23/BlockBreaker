#pragma once
#include <string>

class Texture 
{
public:
	Texture();
	Texture(int width, int height);

	void Generate(unsigned char* data);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	void SetInternalFormat(unsigned int i) { m_InternalFormat = i; }
	void SetImageFormat(unsigned int i) { m_ImageFormat = i; }

	void Clean();

private:
	unsigned int m_RendererID;
	int m_Width, m_Height;
	// texture Format
	unsigned int m_InternalFormat; // format of texture object
	unsigned int m_ImageFormat; // format of loaded image
	// texture configuration
	unsigned int m_Wrap_S; // wrapping mode on S axis
	unsigned int m_Wrap_T; // wrapping mode on T axis
	unsigned int m_FilterMin; // filtering mode if texture pixels < screen pixels
	unsigned int m_FilterMax; // filtering mode if texture pixels > screen pixels
};