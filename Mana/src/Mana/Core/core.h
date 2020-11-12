#pragma once

#include <memory>

#ifdef _WIN32
    /* Windows x64/x86 */
    #ifdef _WIN64
        /* Windows x64  */
        #define MA_PLATFORM_WINDOWS
    #else
    /* Windows x86 */
    #error "x86 Builds are not supported!"
    #endif
#endif

#ifdef MA_DEBUG
    #if defined(MA_PLATFORM_WINDOWS)
        #define MA_DEBUGBREAK() __debugbreak()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define MA_ENABLE_ASSERTS
#else
#   define HZ_DEBUGBREAK()
#endif

#ifdef MA_DEBUG
    #define MA_ENABLE_ASSERTS
#endif

#ifdef MA_ENABLE_ASSERTS
    #define MA_ASSERT(x, ...) {if(!(x)) {MA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
    #define MA_CORE_ASSERT(x, ...) {if(!(x)) {MA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
    #define MA_ASSERT(x, ...)
    #define MA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Mana {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}