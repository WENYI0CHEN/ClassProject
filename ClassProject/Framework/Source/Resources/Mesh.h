#include "Core/CoreHeaders.h"
#include "Math/vec2.h"
#include "Color.h"

namespace fw
{
	class Camera;
	class ShaderProgram;
	class Texture;

	struct VertexFormat
	{
		vec2 pos;
		unsigned char color[4];
		vec2 uv;
	};

	class Mesh
	{
	public:
		Mesh(GLenum primitiveType, const std::vector<VertexFormat>& attribs);
		Mesh(GLenum primitiveType, const std::vector<VertexFormat>& attribs, const std::vector<unsigned int>& indices);
		virtual ~Mesh();

		void rebuild(GLenum primitiveType, const std::vector<VertexFormat>& attribs, const std::vector<unsigned int>& indices = {});
		void draw(Camera* pCamera, ShaderProgram* shader, vec2 scale, float rot, vec2 pos, color4f color, Texture* texture);

	protected:
		// Declare a VBO, which is just a number.
		GLuint m_vbo = 0;
		GLuint m_ibo = 0;

		int m_vertexCount = 0;
		int m_indexCount = 0;
		GLenum m_primitiveType = GL_POINTS;
	};
}
