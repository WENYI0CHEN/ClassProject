#include "Core/CoreHeaders.h"

namespace fw
{
	class Texture
	{
	public:
		Texture();
		Texture(std::string filename);
		~Texture();

		void create(unsigned char* pixels, int width, int height);

		GLuint getTexture() { return m_texture; }

	protected:
		GLuint m_texture = 0;
	};
}
