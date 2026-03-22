#include "normalize_camera_up.h"
#include "component.h"
#include "../transform/component.h"
#include "glm/gtc/quaternion.hpp"

bool camera::system::normalize_camera_up_system::update() noexcept {
  using namespace component;
  using namespace transform::component;

  const auto& view = registry.view<view_matrix_component, rotation_component, position_component>();

  for (auto&& [entity, matrix, orientation, position] : view.each()) {
    auto forward = glm::vec3{matrix.value[0][2], matrix.value[1][2], matrix.value[2][2]};
    orientation.value = glm::lookAt(position.value, position.value + forward, glm::vec3{0, 1.f, 0});
  }

  return true;
}