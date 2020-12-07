#pragma once

#include "Mana.h"

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void loadAssets() = 0;
    virtual void onUpdate(Mana::TimeStep dt) = 0;
    virtual void onRender() = 0;

    virtual void onImGuiRender() = 0;

    virtual void reset() = 0;

    virtual const glm::vec2& getPosition() const = 0;
    virtual const glm::vec2& getSize() const = 0;
};