//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_GRAPHICSCONTEXT_H
#define VIKING_GRAPHICSCONTEXT_H

namespace Viking {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;

        virtual void init() = 0;
        virtual void swapBuffers() = 0;

        static Scope<GraphicsContext> create(void* window);
    };
}

#endif //VIKING_GRAPHICSCONTEXT_H
