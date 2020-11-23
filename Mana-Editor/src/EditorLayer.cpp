#include "EditorLayer.h"

#include "Mana/Render/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include "Mana/Scene/SceneSerialization.h"
#include "Mana/Utilities/PlatformUtilities.h"

namespace Mana {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_cameraController(1920.0f / 1080.0f, true)
    {
    }

    void EditorLayer::onAttach()
    {

        //m_checkerBoard = Texture2D::Create("assets/textures/Checkerboard.png");

        FramebufferSpecs frameSpec;
        frameSpec.width = 1280.0f;
        frameSpec.height = 720.0f;
        m_framebuffer = Framebuffer::Create(frameSpec);

        m_activeScene = CreateRef<Scene>();

#if 0
        auto square = m_activeScene->createEntity("Purple Square");
        square.addComponent<SpriteRendererComponent>(m_Color);

        auto redSquare = m_activeScene->createEntity("Red Square");
        redSquare.addComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.1f, 0.3f, 1.0f));

        auto thirdSquare = m_activeScene->createEntity("Third Square");
        thirdSquare.addComponent<SpriteRendererComponent>(glm::vec4(0.4f, 0.6f, 0.3f, 1.0f));

        m_squareEntity = square;

        m_cameraEntity = m_activeScene->createEntity("Camera A");
        m_cameraEntity.addComponent<CameraComponent>();

        m_SecondCamera = m_activeScene->createEntity("Camera B");
        auto& cc = m_SecondCamera.addComponent<CameraComponent>();
        cc.Primary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            void onCreate()
            {

            }

            void onDestroy()
            {

            }

            void onUpdate(TimeStep dt)
            {
                //std::cout << dt << std::endl;

                auto& translation = getComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if (Input::isKeyPressed(MA_KEY_A))
                    translation.x -= speed * dt;
                if (Input::isKeyPressed(MA_KEY_D))
                    translation.x += speed * dt;
                if (Input::isKeyPressed(MA_KEY_W))
                    translation.y += speed * dt;
                if (Input::isKeyPressed(MA_KEY_S))
                    translation.y -= speed * dt;
            }

        };

        m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
        m_SecondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();

#endif
        m_hierarchyPanel.setContext(m_activeScene);

    }

    void EditorLayer::onDetach()
    {

    }

    void EditorLayer::onUpdate(TimeStep dt)
    {

        if (FramebufferSpecs spec = m_framebuffer->getSpecification();
            m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
            (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
        {
            m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
            m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);

            m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
        }

        if(m_viewportFocused)
            m_cameraController.onUpdate(dt);

        Renderer2D::resetStats();
        m_framebuffer->bind();
        RenderCommand::setClearColor({ 0.15f, 0.15f, 0.15f, 1 });
        RenderCommand::clear();
        

        //update the scene
        m_activeScene->onUpdate(dt);

        m_framebuffer->unbind();

    }

    void EditorLayer::onImGuiRender()
    {

        // Note: Switch this to true to enable dockspace
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
             
                if (ImGui::MenuItem("New", "ctrl+N"))
                {
                    newScene();
                }

                if (ImGui::MenuItem("Open...", "ctrl+O"))
                {
                    openScene();
                }

                if (ImGui::MenuItem("Save As...", "ctrl+shift+S"))
                {
                    saveSceneAs();
                }

                if (ImGui::MenuItem("Exit")) 
                    Mana::Application::get().close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        m_hierarchyPanel.onImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->setBlockEvents(!m_viewportFocused || !m_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        uint64_t textureID = m_framebuffer->getColorAttachmentRendererId();
        ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::onEvent(Event& event)
    {
        m_cameraController.onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(MA_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent& event)
    {
        if (event.getRepeatCount() > 0)
            return false;

        bool ctrlPressed = Input::isKeyPressed(MA_KEY_LEFT_CONTROL) || Input::isKeyPressed(MA_KEY_LEFT_CONTROL);
        bool shiftPressed = Input::isKeyPressed(MA_KEY_LEFT_CONTROL) || Input::isKeyPressed(MA_KEY_LEFT_CONTROL);
        switch (event.getKeyCode())
        {
        case MA_KEY_N:
        {
            if (ctrlPressed)
                newScene();

        }
        break;
        case MA_KEY_O:
        {
            if (ctrlPressed)
                openScene();
        }
        break;
        case MA_KEY_S:
        {
            if (ctrlPressed && shiftPressed)
                saveSceneAs();

        }
        default:
            break;
        }
    }

    void EditorLayer::newScene()
    {
        m_activeScene = CreateRef<Scene>();
        m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, m_viewportSize.y);
        m_hierarchyPanel.setContext(m_activeScene);
    }

    void EditorLayer::openScene()
    {
        std::string filePath = FileDialogs::openFile("Mana Scene (*.mana)\0*.mana\0");
        if (!filePath.empty())
        {
            m_activeScene = CreateRef<Scene>();
            m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, m_viewportSize.y);
            m_hierarchyPanel.setContext(m_activeScene);

            SceneSerialization serializer(m_activeScene);
            serializer.deserialize(filePath);
        }
    }

    void EditorLayer::saveSceneAs()
    {
        std::string filePath = FileDialogs::saveFile("Mana Scene (*.mana)\0*.mana\0");

        if (!filePath.empty())
        {
            SceneSerialization serializer(m_activeScene);
            serializer.serialize(filePath);
        }
    }

}
