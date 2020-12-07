#pragma once

#include "Mana.h"

#include "Scene.h"

class GameLayer : public Mana::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    void onUpdate(Mana::TimeStep dt) override;
    virtual void onImGuiRender() override;

    void onEvent(Mana::Event& event) override;
    bool onWindowResize(Mana::WindowResizeEvent& event);
private:
    void createCamera(uint32_t width, uint32_t height);

private:
    Mana::Scope<Mana::OrthographicCamera> m_camera;
    Scene m_scene;

    float m_time = 0.0f;
    

};