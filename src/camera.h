#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class camera {
  public:
  glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
  glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
  glm::vec3 right = glm::vec3(1.f, 0.f, 0.f);
  float near_plane = .1f;
  float far_plane = 1000.f;
  glm::mat4 view;
  glm::mat4 projection;
};