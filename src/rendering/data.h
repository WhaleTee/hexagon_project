#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace rendering::data {
  struct vertex {
    glm::vec3 position;
    glm::vec4 color;
  };
} // namespace rendering::data

namespace rendering::data::uniform {
  // Uniform data must respect std140 layout conventions

  struct vertex_uniform {
    glm::vec4 model;
    glm::vec4 view;
    glm::vec4 projection;
    glm::mat4 matrix;
  };

  struct fragment_uniform {
    glm::vec3 light_direction;
    float padding_1;
    glm::vec3 light_color;
    float padding_2;
    glm::vec3 view_position;
    float padding_3;
  };
} // namespace rendering::data::uniform