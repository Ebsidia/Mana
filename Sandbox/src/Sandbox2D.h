#pragma once
#include "Mana.h"


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

    glm::vec2 m_cubePosition = { -1.0f, 0.0f };

    float m_cubeMoveSpeed = 5.0f;

    glm::vec4 m_Color = { 0.33f, 0.0f, 0.66f, 1.0f };
};