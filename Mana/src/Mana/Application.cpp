#include "mapch.h"
#include "Application.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mana/Input.h"

namespace Mana {

    Application* Application::s_instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case Mana::ShaderDataType::Float:   return GL_FLOAT;
            case Mana::ShaderDataType::Float2:  return GL_FLOAT;
            case Mana::ShaderDataType::Float3:  return GL_FLOAT;
            case Mana::ShaderDataType::Float4:  return GL_FLOAT;
            case Mana::ShaderDataType::Mat3:    return GL_FLOAT;
            case Mana::ShaderDataType::Mat4:    return GL_FLOAT;
            case Mana::ShaderDataType::Int:     return GL_INT;
            case Mana::ShaderDataType::Int2:    return GL_INT;
            case Mana::ShaderDataType::Int3:    return GL_INT;
            case Mana::ShaderDataType::Int4:    return GL_INT;
            case Mana::ShaderDataType::Bool:    return GL_BOOL;
        }

        MA_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }

    Application::Application()
    {
        MA_CORE_ASSERT(!s_instance, "Appication already exists!");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->setEventCallback(MA_BIND_EVENT_FN(Application::onEvent));

        m_imguiLayer = new ImGuiLayer;
        pushOverlay(m_imguiLayer);

        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.33f, 0.0f, 0.66f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.66f, 0.33f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.66f, 0.33f, 1.0f
        };

        m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout = {
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float4, "a_color"}
            };

            m_vertexBuffer->setLayout(layout);

        }

        uint32_t index = 0;
        const auto& layout = m_vertexBuffer->getLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.getComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), 
                element.Normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)element.Offset);
            index++;
        }

        uint32_t indices[3] = {
            0, 1, 2
        };

        m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = vec4(a_position, 1.0f);
                
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
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
       
        while (m_running)
        {
            glClearColor(0.15f, 0.15f, 0.15f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->bind();
            glBindVertexArray(m_vertexArray);
            glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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