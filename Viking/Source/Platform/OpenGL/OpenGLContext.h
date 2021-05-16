//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_OPENGLCONTEXT_H
#define VIKING_OPENGLCONTEXT_H

#include "Viking/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Viking {
    class OpenGLContext final: public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        void init() override;
        void swapBuffers() override;
    private:
        GLFWwindow* mWindowHandle;
    };
}

#endif //VIKING_OPENGLCONTEXT_H
