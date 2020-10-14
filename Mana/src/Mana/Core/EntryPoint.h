#pragma once

#ifdef MA_PLATFORM_WINDOWS

//Mana::CreateApplication is an client Application created function.
extern Mana::Application* Mana::CreateApplication();

int main(int argc, char** argv)
{

    Mana::Log::init();
  
    auto app = Mana::CreateApplication();
    app->run();
    delete app;
}


#endif