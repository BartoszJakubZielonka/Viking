//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Viking {

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle): mWindowHandle(windowHandle) {
        VI_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::init() {
        VI_PROFILE_FUNCTION();

        glfwMakeContextCurrent(mWindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VI_CORE_ASSERT(status, "Failed to initialize Glad!");

        VI_CORE_INFO("OpenGL Info:");
        VI_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        VI_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        VI_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        HZ_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::swapBuffers() {
        VI_PROFILE_FUNCTION();

        glfwSwapBuffers(mWindowHandle);
    }
}