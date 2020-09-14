#pragma once

/******************************************************************************
   File: Log.h

   Description:
       Log class for both Mana Game Engine and the Mana Application


*******************************************************************************/

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Mana {

    class MANA_API Log
    {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

    private:
       static std::shared_ptr<spdlog::logger> s_coreLogger;
       static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}


//Core Log Macros
#define MA_CORE_TRACE(...)  ::Mana::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MA_CORE_INFO(...)   ::Mana::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MA_CORE_WARN(...)   ::Mana::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MA_CORE_ERROR(...)  ::Mana::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MA_CORE_FATAL(...)  ::Mana::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Log Macros
#define MA_TRACE(...)  ::Mana::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MA_INFO(...)   ::Mana::Log::GetClientLogger()->info(__VA_ARGS__)
#define MA_WARN(...)   ::Mana::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MA_ERROR(...)  ::Mana::Log::GetClientLogger()->error(__VA_ARGS__)
#define MA_FATAL(...)  ::Mana::Log::GetClientLogger()->critical(__VA_ARGS__)