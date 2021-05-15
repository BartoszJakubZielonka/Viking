//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_INPUT_H
#define VIKING_INPUT_H

#include <glm/glm.hpp>

#include "Viking/Core/KeyCodes.h"
#include "Viking/Core/MouseCodes.h"

namespace Viking {
    class Input {
    public:
        static bool isKeyPressed(KeyCode key);
        static bool isMouseButtonPressed(MouseCode button);

        static glm::vec2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };
}

#endif //VIKING_INPUT_H
