#pragma once

#ifdef MA_PLATFORM_WINDOWS

extern Mana::Application* Mana::CreateApplication();

int main(int argc, char** argv)
{
  
    auto app = Mana::CreateApplication();
    app->run();
    delete app;
}


#endif