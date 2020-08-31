#pragma once

#ifdef MA_PLATFORM_WINDOWS

//Mana::CreateApplication is an client Application created function.
extern Mana::Application* Mana::CreateApplication();

int main(int argc, char** argv)
{

    Mana::Log::init();
  
    MA_CORE_INFO("Mana Engine Initialized");


    auto app = Mana::CreateApplication();
    app->run();
    delete app;
}


#endif