#include "Core/CoreHeaders.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Color.h"
#include "Texture.h"

namespace fw
{
    Mesh::Mesh(GLenum primitiveType, const std::vector<VertexFormat>& attribs)
    {
        rebuild( primitiveType, attribs );
    }

    Mesh::Mesh(GLenum primitiveType, const std::vector<VertexFormat>& attribs,
        const std::vector<unsigned int>& indices)
    {
        rebuild( primitiveType, attribs, indices );
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers( 1, &m_vbo );
        glDeleteBuffers( 1, &m_ibo );
    }

    void Mesh::rebuild(GLenum primitiveType, const std::vector<VertexFormat>& attribs,
        const std::vector<unsigned int>& indices)
    {
        glDeleteBuffers( 1, &m_vbo );
        glDeleteBuffers( 1, &m_ibo );

        m_primitiveType = primitiveType;
        m_vertexCount = (int)attribs.size();
        m_indexCount = (int)indices.size();

        // Generate a buffer for our vertex attributes.
        // Set this VBO to be the currently active one.
        // Copy our attribute data into the VBO.
        // Once copied into the VBO, the stack copy above isn't needed anymore.
        glGenBuffers( 1, &m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_vertexCount, attribs.data(), GL_STATIC_DRAW );

        // Same for IBO, if needed.
        if( !indices.empty() )
        {
            glGenBuffers( 1, &m_ibo );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                sizeof(unsigned int) * m_indexCount, indices.data(), GL_STATIC_DRAW );
        }
    }

    void Mesh::draw(Camera* pCamera, ShaderProgram* shader, vec2 scale, float rot, vec2 pos, color4f color, Texture* texture)
    {
        glUseProgram( shader->getProgram() );

        GLint scaleLocation = glGetUniformLocation( shader->getProgram(), "u_scale" );
        GLint angleLocation = glGetUniformLocation( shader->getProgram(), "u_angle" );
        GLint offsetLocation = glGetUniformLocation( shader->getProgram(), "u_offset" );
        GLint colorLocation = glGetUniformLocation( shader->getProgram(), "u_color" );
        GLint cameraPosLocation = glGetUniformLocation( shader->getProgram(), "u_cameraPosition" );
        GLint projScaleLocation = glGetUniformLocation( shader->getProgram(), "u_projectionScale" );
        GLint texLocation = glGetUniformLocation( shader->getProgram(), "u_texture" );

        glUniform2f( scaleLocation, scale.x, scale.y );
        glUniform1f( angleLocation, rot );
        glUniform2f( offsetLocation, pos.x, pos.y );
        glUniform3f( colorLocation, color.r, color.g, color.b );
        glUniform2f( cameraPosLocation, pCamera->getPosition().x, pCamera->getPosition().y );
        glUniform2f( projScaleLocation, pCamera->getProjScale().x, pCamera->getProjScale().y );

        // Setup texture uniform.
        if( texture != nullptr )
        {
            int textureUnit = 0;
            glActiveTexture( GL_TEXTURE0 + textureUnit );
            glBindTexture( GL_TEXTURE_2D, texture->getTexture() );
            glUniform1i( texLocation, textureUnit );

        }

        // Set this VBO to be the currently active one.
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );

        // Get the attribute variable’s location from the shader.
        GLint loc = glGetAttribLocation( shader->getProgram(), "a_position" );
        glEnableVertexAttribArray( loc );
        glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,pos) );

        GLint locColor = glGetAttribLocation( shader->getProgram(), "a_color" );
        glEnableVertexAttribArray( locColor );
        glVertexAttribPointer( locColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,color) );

        GLint locUV = glGetAttribLocation( shader->getProgram(), "a_uv" );
        glEnableVertexAttribArray( locUV );
        glVertexAttribPointer( locUV, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat,uv) );

        // Draw the primitive.
        if( m_indexCount == 0 )
        {
            glDrawArrays( m_primitiveType, 0, m_vertexCount );
        }
        else
        {
            glDrawElements( m_primitiveType, m_indexCount, GL_UNSIGNED_INT, nullptr );
        }
    }
}
