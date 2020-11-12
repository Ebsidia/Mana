#pragma once
#include "Mana.h"

namespace Mana {
    class EditorLayer : public Mana::Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onUpdate(Mana::TimeStep dt) override;
        virtual void onImGuiRender() override;
        void onEvent(Mana::Event& event) override;
    private:
        OrthographicCameraController m_cameraController;

        Ref<VertexArray> m_squareVA;
        Ref<Shader> m_flatColorShader;
        Ref<Framebuffer> m_framebuffer;
        Ref<Texture2D> m_checkerBoard;

        glm::vec2 m_viewportSize;
        bool m_viewportFocused = false;
        bool m_viewportHovered = false;

        glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
}