#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace rendering::data {
  struct camera {
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 right = glm::vec3(1.f, 0.f, 0.f);
    float near_plane = .1f;
    float far_plane = 1000.f;
    glm::mat4 view;
    glm::mat4 projection;
  };

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