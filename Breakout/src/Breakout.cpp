#include "Mana.h"
#include "Mana/Core/EntryPoint.h"

#include "GameLayer.h"

class Breakout : public Mana::Application
{
public:
    Breakout()
    {
        //pushLayer(new ExampleLayer());
        pushLayer(new GameLayer());
    }

    ~Breakout()
    {

    }
};

//Required function defined in the Application from Application.h
Mana::Application* Mana::CreateApplication()
{
    return new Breakout();
}