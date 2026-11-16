#pragma once

#include "GameDataTypes.h"

class ImGuiManager;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore* framework);
    virtual ~Game();

    void reset();
    virtual void update(float deltaTime) override;
    virtual void draw() override;
    virtual void onKey(int keyCode, bool pressed) override;

    fw::ShaderProgram* getShader() { return m_pBasicShader; }

    fw::Mesh* getMesh(const char* meshName) const
    {
        auto it = m_meshes.find(meshName);
        if( it != m_meshes.end() )
        {
            return it->second;
        }
        //assert( false );
        return nullptr;
    }

    fw::Texture* getTexture(const char* textureName) const
    {
        auto it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }
        //assert( false );
        return nullptr;
    }

private:
    ImGuiManager* m_pImGuiManager = nullptr;

    std::map<std::string, fw::Mesh*> m_meshes;
    std::map<std::string, fw::Texture*> m_textures;
    fw::ShaderProgram* m_pBasicShader = nullptr;
    fw::ShaderProgram* m_pTextureShader = nullptr;
};
