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
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
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