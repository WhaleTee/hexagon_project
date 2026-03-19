#pragma once
#include "glm/detail/type_quat.hpp"
#include "glm/fwd.hpp"

namespace space::component {
  struct field_of_view_component {
    float value; // degrees
  };

  struct position_component {
    glm::vec3 value;
  };

  struct orientation_component {
    glm::mat4 value;
  };

  struct rotation_request {
    float x, y, z;
  };
} // namespace space::component