#include "perspective_projection.h"
#include "../component_tag.h"
#include "../transform/component.h"
#include "component.h"
#include "glm/ext/matrix_clip_space.hpp"

bool camera::system::camera_perspective_projection_system::update() noexcept {
  using namespace component;
  using namespace transform::component;

  const auto& view = registry.view<projection_component, camera_setting_component, field_of_view_component, entt::tag<component_tag::perspective_projection>, set_projection_request>();

  if (registry.valid(view.front())) mark_to_destroy();

  for (auto&& [entity, projection, settings, fov]: view.each()) {
    const auto& width = static_cast<float>(settings.width);
    const auto& height = static_cast<float>(settings.height);

    projection.value = glm::perspective(glm::radians(fov.value), width / height, settings.near, settings.far);
  }

  return true;
}