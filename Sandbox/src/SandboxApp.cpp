#include "Mana.h"

#include "imgui/imgui.h"

class ExampleLayer : public Mana::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f)
    {
        m_vertexArray.reset(Mana::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.33f, 0.0f, 0.66f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.66f, 0.33f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.66f, 0.33f, 1.0f
        };

        std::shared_ptr<Mana::VertexBuffer> vertexBuffer;
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

        std::shared_ptr<Mana::IndexBuffer> indexBuffer;
        indexBuffer.reset(Mana::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_vertexArray->setIndexBuffer(indexBuffer);

        m_squareVA.reset(Mana::VertexArray::Create());

        float square[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f,
        };

        std::shared_ptr<Mana::VertexBuffer> squareVB;
        squareVB.reset(Mana::VertexBuffer::Create(square, sizeof(square)));

        Mana::BufferLayout squarelayout = {
            {Mana::ShaderDataType::Float3, "a_position"},
        };

        squareVB->setLayout({
            {Mana::ShaderDataType::Float3, "a_position"}
            });

        m_squareVA->addVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        std::shared_ptr<Mana::IndexBuffer> squareIB;
        squareIB.reset(Mana::IndexBuffer::Create(squareIndices, sizeof(square) / sizeof(uint32_t)));
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

        m_shader.reset(new Mana::Shader(vertexSource, fragmentSource));

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

        m_squareShader.reset(new Mana::Shader(squareVertSource, squareFragSource));
    }

    void onUpdate() override
    {
        if(Mana::Input::isKeyPressed(MA_KEY_A))
            m_cameraPosition.x -= m_cameraMoveSpeed;
        else if (Mana::Input::isKeyPressed(MA_KEY_D))
            m_cameraPosition.x += m_cameraMoveSpeed;

        if (Mana::Input::isKeyPressed(MA_KEY_W))
            m_cameraPosition.y += m_cameraMoveSpeed;
        else if (Mana::Input::isKeyPressed(MA_KEY_S))
            m_cameraPosition.y -= m_cameraMoveSpeed;

        if (Mana::Input::isKeyPressed(MA_KEY_LEFT))
            m_cameraPosition.x -= m_cameraMoveSpeed;
        else if (Mana::Input::isKeyPressed(MA_KEY_RIGHT))
            m_cameraPosition.x += m_cameraMoveSpeed;

        if (Mana::Input::isKeyPressed(MA_KEY_UP))
            m_cameraPosition.y += m_cameraMoveSpeed;
        else if (Mana::Input::isKeyPressed(MA_KEY_DOWN))
            m_cameraPosition.y -= m_cameraMoveSpeed;

        if (Mana::Input::isKeyPressed(MA_KEY_Q))
            m_cameraRotation += m_cameraRotationSpeed;
        else if (Mana::Input::isKeyPressed(MA_KEY_E))
            m_cameraRotation -= m_cameraRotationSpeed;

        Mana::RenderCommand::setClearColor({ 0.15f, 0.15f, 0.15f, 1 });
        Mana::RenderCommand::clear();

        m_camera.setPosition(m_cameraPosition);
        m_camera.setRotaion(m_cameraRotation);

        Mana::Renderer::beginScene(m_camera);

        Mana::Renderer::submit(m_squareShader, m_squareVA);
        Mana::Renderer::submit(m_shader, m_vertexArray);

        Mana::Renderer::endScene();
    }

    virtual void onEvent(Mana::Event& event) override
    {
        /*if (event.getEventType() == Mana::EventType::KeyPressed)
        {
            Mana::KeyPressedEvent& e = (Mana::KeyPressedEvent & )event;
            MA_TRACE("{0}", (char)e.getKeyCode());
        }*/
    }

    void onImGuiRender() override
    {
        
    }

private:
    std::shared_ptr<Mana::Shader> m_shader;
    std::shared_ptr<Mana::VertexArray> m_vertexArray;

    //test
    std::shared_ptr<Mana::Shader> m_squareShader;
    std::shared_ptr<Mana::VertexArray> m_squareVA;

    Mana::OrthographicCamera m_camera;
    glm::vec3 m_cameraPosition;
    float m_cameraMoveSpeed = 0.1f;

    float m_cameraRotation = 0.0f;
    float m_cameraRotationSpeed = 2.0f;
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