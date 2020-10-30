#pragma once
#include "Mana.h"

#include "ParticleSystem.h"

class Sandbox2D : public Mana::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate(Mana::TimeStep dt) override;
    virtual void onImGuiRender() override;
    void onEvent(Mana::Event& event) override;
private:
    Mana::OrthographicCameraController m_cameraController;

    Mana::Ref<Mana::VertexArray> m_squareVA;
    Mana::Ref<Mana::Shader> m_flatColorShader;
    Mana::Ref<Mana::Texture2D> m_checkerBoard;
    Mana::Ref<Mana::Texture2D> m_mario;
    Mana::Ref<Mana::Texture2D> m_spriteSheet;

    glm::vec2 m_cubePosition = { -1.0f, 1.0f };

    float m_cubeMoveSpeed = 4.0f;

    glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

    ParticleSystem m_particleSystem;
    ParticleProps m_particle;
};