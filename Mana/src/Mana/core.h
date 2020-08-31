#pragma once



#ifdef MA_PLATFORM_WINDOWS
    #ifdef MA_BUILD_DLL
        #define MANA_API __declspec(dllexport)
    #else
        #define MANA_API __declspec(dllimport)
    #endif
#else
    #error Mana only supports Windows!
#endif