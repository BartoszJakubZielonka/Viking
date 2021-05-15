//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_MATH_H
#define VIKING_MATH_H

#include <glm/glm.hpp>

namespace Viking::Math {
    bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}

#endif //VIKING_MATH_H
