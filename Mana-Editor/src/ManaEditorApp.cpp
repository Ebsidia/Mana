#include "Mana.h"

#include "Mana/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Mana {

    class ManaEditor : public Application
    {
    public:
        ManaEditor()
            : Application("Mana Editor")
        {
            pushLayer(new EditorLayer());
        }

        ~ManaEditor()
        {

        }
    };

    //Required function defined in the Application from Application.h
    Mana::Application* CreateApplication()
    {
        return new ManaEditor();
    }

}