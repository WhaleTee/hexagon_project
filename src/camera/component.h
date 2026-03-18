#pragma once
#include "SDL3/SDL_stdinc.h"
#include "glm/mat4x4.hpp"

namespace camera::component {
  struct camera_setting_component {
    Uint32 width;
    Uint32 height;
    float near;
    float far;
  };

  struct projection_component {
    glm::mat4 value;
  };

  struct view_matrix_component {
    glm::mat4 value;
  };
} // namespace camera::component