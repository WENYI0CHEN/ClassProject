#pragma once

struct ImDrawData;

class ImGuiManager
{
public:
    ImGuiManager(fw::FWCore* pFramework);
    virtual ~ImGuiManager();

    void onFocusLost();

    void startFrame(float deltaTime);
    void endFrame();

    void onInputCharacter(unsigned int character);
    
protected:
    void init();
    void shutdown();

    void clearInput();

    void renderDrawLists(ImDrawData* draw_data);

    bool createFontsTexture();
    bool createDeviceObjects();
    void invalidateDeviceObjects();

protected:
    fw::FWCore* m_pFramework;

    GLuint m_fontTexture;
    int m_shaderHandle;
    int m_vertHandle;
    int m_fragHandle;
    int m_attribLocationTex;
    int m_attribLocationProjMtx;
    int m_attribLocationPosition;
    int m_attribLocationUV;
    int m_attribLocationColor;
    unsigned int m_VBOHandle;
    unsigned int m_VAOHandle;
    unsigned int m_elementsHandle;
};
