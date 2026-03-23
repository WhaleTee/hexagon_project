#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"

namespace transform::component {
  struct field_of_view_component {
    float value; // degrees
  };

  struct orientation_component {
    glm::quat value;
  };

  struct model_matrix_component {
    glm::mat4 value;
  };

  struct parent_model_matrix_component {
    glm::mat4 value;
  };

  struct position_component {
    glm::vec3 value;
  };

  struct rotation_component {
    glm::quat value;
  };

  struct scale_component {
    glm::vec3 value;
  };

  struct translation_request {
    glm::vec3 value;
  };

  struct rotation_request {
    glm::vec3 value;
    bool local;
  };

  struct scale_request {
    glm::vec3 value;
  };
} // namespace space::component