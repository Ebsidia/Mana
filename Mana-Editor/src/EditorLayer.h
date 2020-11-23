#pragma once
#include "Mana.h"
#include "Panels/SceneHierarchyPanel.h"

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
        bool onKeyPressed(KeyPressedEvent& event);

        void newScene();
        void saveSceneAs();
        void openScene();

    private:
        OrthographicCameraController m_cameraController;

        Ref<VertexArray> m_squareVA;
        Ref<Shader> m_flatColorShader;
        Ref<Framebuffer> m_framebuffer;

        Ref<Scene> m_activeScene;
        Entity m_squareEntity;
        Entity m_blueSquare;
        Entity m_cameraEntity;
        Entity m_SecondCamera;

        bool m_primaryCamera = true;

        Ref<Texture2D> m_checkerBoard;

        glm::vec2 m_viewportSize{0.0f, 0.0f};
        bool m_viewportFocused = false;
        bool m_viewportHovered = false;

        glm::vec4 m_Color = { 0.33f, 0.0f, 0.90f, 1.0f };

        SceneHierarchyPanel m_hierarchyPanel;
    };
}