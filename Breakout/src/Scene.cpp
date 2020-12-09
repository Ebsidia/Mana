#include "Scene.h"

#include <irrKlang.h>

const glm::vec2 INITIAL_BALL_VELOCITY(200.0F, -450.0f);

irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

void Scene::init()
{
    soundEngine->setSoundVolume(0.3f);
    ImGuiIO io = ImGui::GetIO();
    m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 120.0f);

    soundEngine->play2D("assets/Breakout/audio/breakout.mp3", true);

    m_background = Mana::Texture2D::Create("assets/Breakout/textures/background.png");
    m_player.loadAssets();
    m_ball.loadAssets();

    m_player.setPosition(glm::vec2(1600 / 2 + m_player.getSize().x / 2, 900 - m_player.getSize().y));
    m_ball.setPosition(m_player.getPosition() + glm::vec2(m_player.getSize().x / 2 - m_ball.getRadius() * 4, -m_ball.getRadius() * 2));
    m_ball.setVelocity(INITIAL_BALL_VELOCITY);

    Level levelOne;
    levelOne.loadLevel("assets/Breakout/LevelData/level-two.txt", 1600, 900 * 0.5);

    m_levels.push_back(levelOne);
    m_currentLevel = 0;
}

void Scene::onUpdate(Mana::TimeStep dt)
{
    switch (m_state)
    {
        case GameState::Play:
        {
            if(m_ball.getIsStuck())
                m_ball.setPosition(m_player.getPosition() + glm::vec2(m_player.getSize().x / 2 - m_ball.getRadius() * 4, -m_ball.getRadius() * 2));

            m_player.onUpdate(dt);
            m_ball.onUpdate(dt);

            if(!m_ball.getIsStuck())
                handleCollision();
            break;
        }
    }

    /*m_player.onUpdate(dt);
    m_ball.onUpdate(dt);*/
}

void Scene::onRender()
{
    Mana::Renderer2D::drawQuad({800.0, 450.0f, -0.8}, { 1600.0f, 900.0f }, m_background);

    m_levels[m_currentLevel].onRender();
    m_player.onRender();
    m_ball.onRender();
}

void Scene::onImGuiRender()
{
    switch (m_state)
    {
    case GameState::Play:
    {

        //ImGui::GetForegroundDrawList()->AddText(m_font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
        break;
    }
    case GameState::MainMenu:
    {
        auto pos = ImGui::GetWindowPos();
        auto width = Mana::Application::get().getWindow().getWidth();
        auto height = Mana::Application::get().getWindow().getHeight();
        pos.x += width * 0.5f - 300.0f;
        pos.y += 50.0f;
        ImGui::GetForegroundDrawList()->AddText(m_font, 120.0f, pos, 0xffffffff, "Click to Play!");
        break;
    }
    case GameState::GameOver:
    {
        auto pos = ImGui::GetWindowPos();
        auto width = Mana::Application::get().getWindow().getWidth();
        auto height = Mana::Application::get().getWindow().getHeight();
        pos.x += width * 0.5f - 300.0f;
        pos.y += 50.0f;

        pos.x += 200.0f;
        pos.y += 150.0f;
        
        break;
    }
    }
}

void Scene::reset()
{
    m_gameOver = false;
    m_player.reset();
}

void Scene::onEvent(Mana::Event& event)
{

    Mana::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Mana::MouseButtonPressedEvent>(MA_BIND_EVENT_FN(Scene::onMouseButtonPressed));
}

bool Scene::onMouseButtonPressed(Mana::MouseButtonEvent& event)
{

    if (m_state == GameState::GameOver)
        reset();

    m_state = GameState::Play;
    return false;
}

DIRECTION Scene::vectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f,  1.0f), // up
        glm::vec2(1.0f,  0.0f), // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f,  0.0f), // left
    };

    float max = 0.0f;
    unsigned int best_match = -1;

    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    return (DIRECTION)best_match;
}

void Scene::handleCollision()
{

    for (Bricks& brick : m_levels[m_currentLevel].getBricks())
    {
        if (!brick.isDestroyed())
        {
            Collision collision = checkCollision(m_ball, brick);
            if (std::get<0>(collision))
            {
                glm::vec2 pos = m_ball.getPosition();

                if (!brick.isSolid())
                {
                    brick.setDestroyed(true);
                    soundEngine->play2D("assets/Breakout/audio/bleep.mp3", false);
                }
                else
                {
                    soundEngine->play2D("assets/Breakout/audio/solid.wav", false);
                }

                DIRECTION dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT)
                {
                    m_ball.setVelocity({ -m_ball.getVelocity().x, m_ball.getVelocity().y });
                    
                    float penetration = m_ball.getRadius() - std::abs(diff_vector.x);
                    if (dir == LEFT)
                    {
                        MA_INFO("LEFT");
                        pos.x += penetration;
                        m_ball.setPosition(pos);
                    }
                    else
                    {
                        MA_INFO("RIGHT");
                        pos.x -= penetration;
                        m_ball.setPosition(pos);
                    }
                }
                else
                {
                    m_ball.setVelocity({ m_ball.getVelocity().x, -m_ball.getVelocity().y });
                   
                    float penetration = m_ball.getRadius() - std::abs(diff_vector.y);
                    if (dir == UP)
                    {
                        MA_INFO("UP");
                        pos.y -= penetration;
                        m_ball.setPosition(pos);
                    }
                    else
                    {
                        MA_INFO("Down");
                        pos.y += penetration;
                        m_ball.setPosition(pos);
                    }
                }
            }
        }
    }

    //check paddle collision
    Collision result = checkCollision(m_ball, m_player);
    if (!m_ball.getIsStuck() && std::get<0>(result))
    {
        MA_INFO("PADDLE");
        float centerBoard = m_player.getPosition().x; //+ m_player.getSize().x / 2.0f;
        float distance = (m_ball.getPosition().x) - centerBoard;
        float percentage = distance / (m_player.getSize().x / 2.0f);

        float strength = 2.5f;
        glm::vec2 oldVelocity = m_ball.getVelocity();
        m_ball.setVelocity({ INITIAL_BALL_VELOCITY.x * percentage * strength, m_ball.getVelocity().y });

        glm::vec2 velocity = glm::normalize(m_ball.getVelocity()) * glm::length(oldVelocity);
        m_ball.setVelocity(velocity);
       
        m_ball.setVelocity({ m_ball.getVelocity().x, -1 * abs(m_ball.getVelocity().y) });
        soundEngine->play2D("assets/Breakout/audio/bleep.wav", false);
    }
}

bool Scene::checkCollision(GameObject& one, GameObject& two)
{
    MA_INFO("Check Collisions");
    //collision on x_axis
    bool collisionX = one.getPosition().x + one.getSize().x >= two.getPosition().x &&
        two.getPosition().x + two.getSize().x >= one.getPosition().x;

    bool collisionY = one.getPosition().y + one.getSize().y >= two.getPosition().y &&
        two.getPosition().y + two.getSize().y >= one.getPosition().y;

    return collisionX && collisionY;
}

Collision Scene::checkCollision(Ball& one, GameObject& two)
{
    glm::vec2 center(one.getPosition());

    glm::vec2 aabb_half_extents(two.getSize().x / 2, two.getSize().y / 2);

    glm::vec2 aabb_center(two.getPosition().x, two.getPosition().y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;

    if (glm::length(difference) < one.getRadius())
        return std::make_tuple(true, vectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0, 0));
}

void Scene::gameOver()
{
    m_gameOver = true;
}
