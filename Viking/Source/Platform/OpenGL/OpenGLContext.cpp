//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Viking {

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle): mWindowHandle(windowHandle) {
        VI_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::init() {
        VI_PROFILE_FUNCTION();

        glfwMakeContextCurrent(mWindowHandle);
        const int status = gladLoadGL(glfwGetProcAddress);
        VI_CORE_ASSERT(status, "Failed to initialize Glad!");

        VI_CORE_INFO("OpenGL Info:");
        VI_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        VI_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        VI_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        VI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Viking requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::swapBuffers() {
        VI_PROFILE_FUNCTION();

        glfwSwapBuffers(mWindowHandle);
    }
}
