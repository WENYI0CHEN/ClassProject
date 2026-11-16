#include "Framework.h"

#include "ImGuiManager.h"
#include "imgui.h"

ImGuiManager::ImGuiManager(fw::FWCore* pFramework)
{
    m_pFramework = pFramework;

    m_fontTexture = 0;
    m_shaderHandle = 0; m_vertHandle = 0; m_fragHandle = 0;
    m_attribLocationTex = 0; m_attribLocationProjMtx = 0;
    m_attribLocationPosition = 0; m_attribLocationUV = 0; m_attribLocationColor = 0;
    m_VBOHandle = 0; m_VAOHandle = 0; m_elementsHandle = 0;

    init();
}

ImGuiManager::~ImGuiManager()
{
    shutdown();
}

void ImGuiManager::init()
{
    ImGui::CreateContext();

    createDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = 600;
    io.DisplaySize.y = 600;
    io.IniFilename = "imgui.ini";
}

void ImGuiManager::shutdown()
{
    ImGui::DestroyContext();
}

void ImGuiManager::clearInput()
{
    ImGuiIO& io = ImGui::GetIO();

    for( int i=0; i<5; i++ )
    {
        io.MouseDown[i] = false;
    }

    io.KeyCtrl = false;
    io.KeyShift = false;
    io.KeyAlt = false;
    io.KeySuper = false;
}

void ImGuiManager::onFocusLost()
{
    clearInput();
}

void ImGuiManager::startFrame(float deltaTime)
{
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = deltaTime;
    io.DisplaySize.x = (float)m_pFramework->getWindowWidth();
    io.DisplaySize.y = (float)m_pFramework->getWindowHeight();

    io.KeyCtrl = m_pFramework->isKeyDown(VK_CONTROL) || m_pFramework->isKeyDown(VK_LCONTROL) || m_pFramework->isKeyDown(VK_RCONTROL);
    io.KeyShift = m_pFramework->isKeyDown(VK_SHIFT) || m_pFramework->isKeyDown(VK_LSHIFT) || m_pFramework->isKeyDown(VK_RSHIFT);
    io.KeyAlt = m_pFramework->isKeyDown(VK_MENU) || m_pFramework->isKeyDown(VK_LMENU) || m_pFramework->isKeyDown(VK_RMENU);
    io.KeySuper = m_pFramework->isKeyDown(VK_LWIN) || m_pFramework->isKeyDown(VK_RWIN);

    int mx, my;
    m_pFramework->getMouseCoordinates( &mx, &my );
    io.MouseDelta.x = (float)mx - io.MousePos.x;
    io.MouseDelta.y = (float)my - io.MousePos.y;
    io.MousePos.x = (float)mx;
    io.MousePos.y = (float)my;
    io.MouseDown[0] = m_pFramework->isMouseButtonDown( 0 );
}

void ImGuiManager::endFrame()
{
    ImGui::Render();
    ImDrawData* data = ImGui::GetDrawData();
    renderDrawLists( data );
}

void ImGuiManager::onInputCharacter(unsigned int character)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter( character );
}

// This is the main rendering function
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImGuiManager::renderDrawLists(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if( fb_width == 0 || fb_height == 0 )
        return;

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLenum last_active_texture; glGetIntegerv( GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture );
    glActiveTexture( GL_TEXTURE0 );
    GLint last_program; glGetIntegerv( GL_CURRENT_PROGRAM, &last_program );
    GLint last_texture; glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
    //GLint last_sampler; glGetIntegerv( GL_SAMPLER_BINDING, &last_sampler );
    GLint last_array_buffer; glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &last_array_buffer );
    GLint last_element_array_buffer; glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer );
    GLint last_vertex_array; glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &last_vertex_array );
    GLint last_polygon_mode[2]; glGetIntegerv( GL_POLYGON_MODE, last_polygon_mode );
    GLint last_viewport[4]; glGetIntegerv( GL_VIEWPORT, last_viewport );
    GLint last_scissor_box[4]; glGetIntegerv( GL_SCISSOR_BOX, last_scissor_box );
    GLenum last_blend_src_rgb; glGetIntegerv( GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb );
    GLenum last_blend_dst_rgb; glGetIntegerv( GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb );
    GLenum last_blend_src_alpha; glGetIntegerv( GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha );
    GLenum last_blend_dst_alpha; glGetIntegerv( GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha );
    GLenum last_blend_equation_rgb; glGetIntegerv( GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb );
    GLenum last_blend_equation_alpha; glGetIntegerv( GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha );
    GLboolean last_enable_blend = glIsEnabled( GL_BLEND );
    GLboolean last_enable_cull_face = glIsEnabled( GL_CULL_FACE );
    GLboolean last_enable_depth_test = glIsEnabled( GL_DEPTH_TEST );
    GLboolean last_enable_scissor_test = glIsEnabled( GL_SCISSOR_TEST );

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable( GL_BLEND );
    //glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_SCISSOR_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Setup viewport, orthographic projection matrix
    glViewport( 0, 0, (GLsizei)fb_width, (GLsizei)fb_height );
    const float ortho_projection[4][4] =
    {
        { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
        { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
        { 0.0f,                  0.0f,                  -1.0f, 0.0f },
        {-1.0f,                  1.0f,                   0.0f, 1.0f },
    };
    glUseProgram( m_shaderHandle );
    glUniform1i( m_attribLocationTex, 0 );
    glUniformMatrix4fv( m_attribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0] );
    glBindVertexArray( m_VAOHandle );
    //glBindSampler( 0, 0 ); // Rely on combined texture/sampler state.

    for( int n = 0; n < draw_data->CmdListsCount; n++ )
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer( GL_ARRAY_BUFFER, m_VBOHandle );
        glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementsHandle );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW );

        for( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if( pcmd->UserCallback )
            {
                pcmd->UserCallback( cmd_list, pcmd );
            }
            else
            {
                glBindTexture( GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TexRef.GetTexID() );
                glScissor( (int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y) );
                glDrawElements( GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset );
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram( last_program );
    glBindTexture( GL_TEXTURE_2D, last_texture );
    //glBindSampler( 0, last_sampler );
    glActiveTexture( last_active_texture );
    glBindVertexArray( last_vertex_array );
    glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer );
    //glBlendEquationSeparate( last_blend_equation_rgb, last_blend_equation_alpha );
    glBlendFuncSeparate( last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha );
    if( last_enable_blend ) glEnable( GL_BLEND ); else glDisable( GL_BLEND );
    if( last_enable_cull_face ) glEnable( GL_CULL_FACE ); else glDisable( GL_CULL_FACE );
    if( last_enable_depth_test ) glEnable( GL_DEPTH_TEST ); else glDisable( GL_DEPTH_TEST );
    if( last_enable_scissor_test ) glEnable( GL_SCISSOR_TEST ); else glDisable( GL_SCISSOR_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, last_polygon_mode[0] );
    glViewport( last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3] );
    glScissor( last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3] );
}

bool ImGuiManager::createFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;

    // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
    io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
    glGenTextures( 1, &m_fontTexture);
    glBindTexture( GL_TEXTURE_2D, m_fontTexture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    // Store our identifier
    io.Fonts->TexID = (void*)(unsigned __int64)m_fontTexture;

    // Restore state
    glBindTexture( GL_TEXTURE_2D, last_texture );

    return true;
}

bool ImGuiManager::createDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

    const GLchar *vertex_shader =
        "#version 150\n"
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "	Frag_UV = UV;\n"
        "	Frag_Color = Color;\n"
        "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* fragment_shader =
        "#version 150\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
        "}\n";

    m_shaderHandle = glCreateProgram();
    m_vertHandle = glCreateShader( GL_VERTEX_SHADER );
    m_fragHandle = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( m_vertHandle, 1, &vertex_shader, 0 );
    glShaderSource( m_fragHandle, 1, &fragment_shader, 0 );
    glCompileShader( m_vertHandle );
    glCompileShader( m_fragHandle );
    glAttachShader( m_shaderHandle, m_vertHandle );
    glAttachShader( m_shaderHandle, m_fragHandle );
    glLinkProgram( m_shaderHandle );

    m_attribLocationTex = glGetUniformLocation( m_shaderHandle, "Texture" );
    m_attribLocationProjMtx = glGetUniformLocation( m_shaderHandle, "ProjMtx" );
    m_attribLocationPosition = glGetAttribLocation( m_shaderHandle, "Position" );
    m_attribLocationUV = glGetAttribLocation( m_shaderHandle, "UV" );
    m_attribLocationColor = glGetAttribLocation( m_shaderHandle, "Color" );

    glGenBuffers( 1, &m_VBOHandle );
    glGenBuffers( 1, &m_elementsHandle );

    glGenVertexArrays( 1, &m_VAOHandle );
    glBindVertexArray( m_VAOHandle );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBOHandle );
    glEnableVertexAttribArray( m_attribLocationPosition );
    glEnableVertexAttribArray( m_attribLocationUV );
    glEnableVertexAttribArray( m_attribLocationColor );

    glVertexAttribPointer( m_attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, pos) );
    glVertexAttribPointer( m_attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, uv) );
    glVertexAttribPointer( m_attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, col) );

    createFontsTexture();

    // Restore modified GL state
    glBindTexture( GL_TEXTURE_2D, last_texture );
    glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
    glBindVertexArray( last_vertex_array );

    return true;
}

void ImGuiManager::invalidateDeviceObjects()
{
    if( m_VAOHandle ) glDeleteVertexArrays( 1, &m_VAOHandle );
    if( m_VBOHandle ) glDeleteBuffers( 1, &m_VBOHandle );
    if( m_elementsHandle ) glDeleteBuffers( 1, &m_elementsHandle );
    m_VAOHandle = m_VBOHandle = m_elementsHandle = 0;

    if( m_shaderHandle && m_vertHandle ) glDetachShader( m_shaderHandle, m_vertHandle );
    if( m_vertHandle ) glDeleteShader( m_vertHandle );
    m_vertHandle = 0;

    if( m_shaderHandle && m_fragHandle ) glDetachShader( m_shaderHandle, m_fragHandle );
    if( m_fragHandle ) glDeleteShader( m_fragHandle );
    m_fragHandle = 0;

    if( m_shaderHandle ) glDeleteProgram( m_shaderHandle );
    m_shaderHandle = 0;

    if( m_fontTexture )
    {
        glDeleteTextures( 1, &m_fontTexture );
        ImGui::GetIO().Fonts->TexRef = ImTextureRef();
        m_fontTexture = 0;
    }
}
