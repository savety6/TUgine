#pragma once

#ifdef TUG_PLATFORM_WINDOWS
    #ifdef TUG_BUILD_DLL
        #define TUGINE_API __declspec(dllexport)
    #else
        #define TUGINE_API __declspec(dllimport)
    #endif
#else
    // #error Tugine only supports Windows!
#endif

#ifdef TUG_ENABLE_ASSERTS
    #define TUG_ASSERT(x, ...) { if(!(x)) { TUG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define TUG_CORE_ASSERT(x, ...) { if(!(x)) { TUG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define TUG_ASSERT(x, ...)
    #define TUG_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

