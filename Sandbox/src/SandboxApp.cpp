#include "Mana.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>



class ExampleLayer : public Mana::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
    {
        m_vertexArray.reset(Mana::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.33f, 0.0f, 0.66f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.66f, 0.33f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.66f, 0.33f, 1.0f
        };

        Mana::Ref<Mana::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Mana::VertexBuffer::Create(vertices, sizeof(vertices)));

        Mana::BufferLayout layout = {
            {Mana::ShaderDataType::Float3, "a_position"},
            {Mana::ShaderDataType::Float4, "a_color"}
        };

        vertexBuffer->setLayout(layout);

        m_vertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {
            0, 1, 2
        };

        Mana::Ref<Mana::IndexBuffer> indexBuffer;
        indexBuffer.reset(Mana::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_vertexArray->setIndexBuffer(indexBuffer);

        m_squareVA.reset(Mana::VertexArray::Create());

        float square[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Mana::Ref<Mana::VertexBuffer> squareVB;
        squareVB.reset(Mana::VertexBuffer::Create(square, sizeof(square)));

        squareVB->setLayout({
            {Mana::ShaderDataType::Float3, "a_position"},
            {Mana::ShaderDataType::Float2, "a_texCoord"}
        });

        m_squareVA->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        Mana::Ref<Mana::IndexBuffer> squareIB;
        squareIB.reset(Mana::IndexBuffer::Create(squareIndices, sizeof(square) / sizeof(uint32_t)));
        m_squareVA->setIndexBuffer(squareIB);

        std::string vertexSource = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_viewProjection;
            uniform mat4 u_transform;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0f);
                
            }
        )";

        std::string fragmentSource = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;
            in vec4 v_color;

            void main()
            {
                color = vec4(v_position, 1.0f);
                color = v_color;
            }
        )";

        m_shader = Mana::Shader::Create("triangleShader", vertexSource, fragmentSource);

        std::string flatColorVertSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjection;
            uniform mat4 u_transform;

            out vec3 v_position;

            void main()
            {
                v_position = a_position;
                gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0f);
                
            }
        )";

        std::string flatColorShaderFragSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_position;

            uniform vec3 u_color;

            void main()
            {
                color = vec4(u_color, 1.0f);
            }
        )";

        m_flatColorShader = Mana::Shader::Create("flatColor", flatColorVertSrc, flatColorShaderFragSrc);

       auto textureShader = m_shaderLibrary.load("assets/shaders/Texture.glsl");

        m_texture = Mana::Texture2D::Create("assets/textures/Checkerboard.png");
        m_mario = Mana::Texture2D::Create("assets/textures/mario.png");

        std::dynamic_pointer_cast<Mana::OpenGLShader>(textureShader)->bind();
        std::dynamic_pointer_cast<Mana::OpenGLShader>(textureShader)->uploadUniformInt("u_texture", 0);
    }

    void onUpdate(Mana::TimeStep timeStep) override
    {
        //MA_TRACE("Delta time: {0}s ({1}ms)", timeStep.getSeconds(), timeStep.getMilliseconds());

        m_cameraController.onUpdate(timeStep);

        Mana::RenderCommand::setClearColor({ 0.15f, 0.15f, 0.15f, 1 });
        Mana::RenderCommand::clear();


        Mana::Renderer::beginScene(m_cameraController.getCamera());

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Mana::OpenGLShader>(m_flatColorShader)->bind();
        std::dynamic_pointer_cast<Mana::OpenGLShader>(m_flatColorShader)->uploadUniformFloat3("u_color", m_squareColor);


        for(int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++) 
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Mana::Renderer::submit(m_flatColorShader, m_squareVA, transform);
            }
        }

        auto textureShader = m_shaderLibrary.get("Texture");

        m_texture->bind();
        Mana::Renderer::submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        m_mario->bind();
        Mana::Renderer::submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        //Mana::Renderer::submit(m_shader, m_vertexArray);

        Mana::Renderer::endScene();
    }

    virtual void onEvent(Mana::Event& event) override
    {
        m_cameraController.onEvent(event);

        /*if (event.getEventType() == Mana::EventType::KeyPressed)
        {
            Mana::KeyPressedEvent& e = (Mana::KeyPressedEvent & )event;
            MA_TRACE("{0}", (char)e.getKeyCode());
        }*/
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Settings");

        ImGui::ColorPicker3("Square Color Picker", glm::value_ptr(m_squareColor));

        ImGui::End();
    }

private:
    Mana::ShaderLibrary m_shaderLibrary;
    Mana::Ref<Mana::Shader> m_shader;
    Mana::Ref<Mana::VertexArray> m_vertexArray;

    //test
    Mana::Ref<Mana::Shader> m_flatColorShader;
    Mana::Ref<Mana::VertexArray> m_squareVA;

    Mana::Ref<Mana::Texture2D> m_texture, m_mario;

    Mana::OrthographicCameraController m_cameraController;

    glm::vec3 m_squareColor = { 0.33f, 0.0f, 0.66f};

};

class Sandbox : public Mana::Application
{
public:
    Sandbox()
    {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

//Required function defined in the Application from Application.h
Mana::Application* Mana::CreateApplication()
{
    return new Sandbox();
}