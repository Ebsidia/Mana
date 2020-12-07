#pragma once

#include <tuple>
#include <imgui/imgui.h>

#include "Player.h"
#include "Level.h"
#include "Ball.h"

enum DIRECTION
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};


typedef std::tuple<bool, DIRECTION, glm::vec2> Collision;

class Scene
{
public:
    void init();

    void onUpdate(Mana::TimeStep dt);
    void onRender();

    void onImGuiRender();

    bool isGameOver() const { return m_gameOver; }
    void reset();

    Player& getPlayer() { return m_player; }

    void onEvent(Mana::Event& event);

    bool onMouseButtonPressed(Mana::MouseButtonEvent& event);

    DIRECTION vectorDirection(glm::vec2 target);
private:
    void handleCollision();
    bool checkCollision(GameObject& one, GameObject& two);
    Collision checkCollision(Ball& one, GameObject& two);
    void gameOver();
private:
    ImFont* m_font;

    Mana::Ref<Mana::Texture2D> m_background;
    Player m_player;
    Ball m_ball;

    std::vector<Level> m_levels;
    uint32_t m_currentLevel;

    bool m_gameOver = false;

    enum class GameState
    {
        Play = 0, MainMenu = 1, GameOver = 2
    };

    GameState m_state = GameState::MainMenu;
};