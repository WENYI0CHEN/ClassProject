#include "Core/CoreHeaders.h"
#include "Texture.h"

namespace fw
{
	Texture::Texture()
	{
		unsigned char pixels[16]
		{
			255,0,0,255,    0,255,0,255,
			0,0,255,255,    255,255,255,255,
		};

		create( pixels, 2, 2 );
	}

	Texture::Texture(std::string filename)
	{
		assert( false && "Loading texture from disk not implemented" );
	}

	Texture::~Texture()
	{
	}

	void Texture::create(unsigned char* pixels, int width, int height)
	{
		glGenTextures( 1, &m_texture );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, m_texture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
}
