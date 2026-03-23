#include "view.h"
#include "../transform/component.h"
#include "component.h"
#include "glm/gtc/quaternion.hpp"

bool camera::system::view_matrix_system::update() noexcept {
  using namespace component;
  using namespace transform::component;

  const auto& view = registry.view<rotation_component, position_component, view_matrix_component>();

  for (auto&& [entity, rotation, position, view_matrix]: view.each()) {
    const auto& rotation_matrix = glm::mat4_cast(rotation.value);
    const auto& position_matrix = glm::translate(glm::mat4{1}, -position.value);

    view_matrix.value = rotation_matrix * position_matrix;
  }

  return true;
}