//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_BASE_H
#define VIKING_BASE_H

#include <memory>
#include "Viking/Core/PlatformDetection.h"

#ifdef VI_DEBUG
#if defined(VI_PLATFORM_WINDOWS)
		#define VI_DEBUGBREAK() __debugbreak()
	#elif defined(VI_PLATFORM_LINUX)
		#include <signal.h>
		#define VI_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define VI_ENABLE_ASSERTS
#else
#define VI_DEBUGBREAK()
#endif

#define VI_EXPAND_MACRO(x) x
#define VI_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define VI_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Viking {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#include "Viking/Core/Log.h"
#include "Viking/Core/Assert.h"

#endif //VIKING_BASE_H
