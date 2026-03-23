#pragma once
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace rendering::data {
  struct vertex {
    glm::vec3 position;
    glm::vec4 color;
  };
} // namespace rendering::data