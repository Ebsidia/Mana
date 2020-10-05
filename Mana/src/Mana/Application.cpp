#include "mapch.h"
#include "Application.h"

#include "Log.h"

#include "Mana/Render/Renderer.h"

#include "Mana/Input.h"


#include "Mana/KeyCodes.h"
#include "mana/MouseButtonCodes.h"

namespace Mana {

    Application* Application::s_instance = nullptr;

    Application::Application()
        : m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        MA_CORE_ASSERT(!s_instance, "Appication already exists!");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->setEventCallback(MA_BIND_EVENT_FN(Application::onEvent));

        m_imguiLayer = new ImGuiLayer;
        pushOverlay(m_imguiLayer);

        m_vertexArray.reset(VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.33f, 0.0f, 0.66f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.66f, 0.33f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.66f, 0.33f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float4, "a_color"}
        };

        vertexBuffer->setLayout(layout);

        m_vertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {
            0, 1, 2
        };

        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_vertexArray->setIndexBuffer(indexBuffer);

        m_squareVA.reset(VertexArray::Create());

        float square[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f,
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(square, sizeof(square)));

        BufferLayout squarelayout = {
            {ShaderDataType::Float3, "a_position"},
        };

        squareVB->setLayout({
            {ShaderDataType::Float3, "a_position"}
            });

        m_squareVA->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(square) / sizeof(uint32_t)));
        m_squareVA->setIndexBuffer(squareIB);

        std::string vertexSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_viewProjection;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = u_viewProjection * vec4(a_position, 1.0f);
                
            }
        )";

        std::string fragmentSource = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;
            in vec4 v_color;

            void main()
            {
                color = vec4(v_position * 0.5 + 0.5, 1.0f);
                color = v_color;
            }
        )";

        m_shader.reset(new Shader(vertexSource, fragmentSource));

        std::string squareVertSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjection;

            out vec3 v_position;

            void main()
            {
                v_position = a_position;
                gl_Position = u_viewProjection * vec4(a_position, 1.0f);
                
            }
        )";

        std::string squareFragSource = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;

            void main()
            {
                color = vec4(0.33f, 0.0f, 0.66f, 1.0f);
            }
        )";

        m_squareShader.reset(new Shader(squareVertSource, squareFragSource));
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
        //m_camera.setPosition({ 0.5f, 0.5f, 0.0f });
        //m_camera.setRotaion(270.0f);

        float x = 0.0f;
        float y = 0.0f;

        while (m_running)
        {
           
            RenderCommand::setClearColor({ 0.15f, 0.15f, 0.15f, 1 });
            RenderCommand::clear();

            

            if (Mana::Input::isKeyPressed(MA_KEY_A))
            {
                x += 0.01f;
                m_camera.setPosition({ x, y, 0.0f });
            }

            if (Mana::Input::isKeyPressed(MA_KEY_D))
            {
                x -= 0.01f;
                m_camera.setPosition({ x, y, 0.0f });
            }

            if (Mana::Input::isKeyPressed(MA_KEY_W))
            {
                y -= 0.01f;
                m_camera.setPosition({ x, y, 0.0f });
            }

            if (Mana::Input::isKeyPressed(MA_KEY_S))
            {
                y += 0.01f;
                m_camera.setPosition({ x, y, 0.0f });
            }

            Renderer::beginScene(m_camera);

            Renderer::submit(m_squareShader, m_squareVA);
            Renderer::submit(m_shader, m_vertexArray);

            Renderer::endScene();

            for (Layer* layer : m_layerStack)
                layer->onUpdate();
            

            m_imguiLayer->begin();

            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();

            m_imguiLayer->end();

            //auto [x, y] = Input::getMousPos();
            //MA_CORE_TRACE("{0}, {1}", x, y);
            
            m_window->onUpdate();
        }
    }
    
    void Application::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>(MA_BIND_EVENT_FN(Application::onWindowClosed));

        //MA_CORE_TRACE("{0}", event);

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
        {
            (*--it)->onEvent(event);
            if (event.handled)
                break;
        }
    }

    void Application::pushLayer(Layer* layer)
    {
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* overlay)
    {
        m_layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    bool Application::onWindowClosed(WindowClosedEvent& event)
    {
        MA_CORE_TRACE("{0}", event);
        m_running = false;

        return true;
    }
}