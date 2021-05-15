//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Window.h"

#ifdef VI_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Viking
{
    Scope<Window> Window::create(const WindowProps props) {
#ifdef VI_PLATFORM_WINDOWS
        return createScope<WindowWindow>(props);
#else
        VI_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}