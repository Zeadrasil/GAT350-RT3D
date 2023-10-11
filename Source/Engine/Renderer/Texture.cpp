#include "Texture.h"
#include "Renderer.h"
#include "Core/Logger.h"
#define STD_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

namespace nc
{
	Texture::~Texture()
	{
		if (m_texture) glDeleteTextures(1, &m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer)
	{
		int channels = 0;
		unsigned char* data = stbi_load(filename.c_str(), &m_size.x, &m_size.y, &channels, 0);
		if (!data)
		{
			WARNING_LOG("Could not create surface: " << filename);
			return false;
		}

		glGenTextures(1, &m_texture);
		glBindTexture(m_target, m_texture);

		GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexStorage2D(m_target, 0, internalFormat, m_size.x, m_size.y);
		glTexSubImage2D(m_target, 0, 0, 0, m_size.x, m_size.y, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		return true;
	}



}