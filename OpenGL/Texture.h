#pragma once
#include <string>
#include <GL/glew.h>

class Texture
{
private:
	
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	
	int m_Width, m_Height, m_BPP;

public:
	unsigned int m_RendererID;
	
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};