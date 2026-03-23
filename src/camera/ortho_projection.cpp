#include "ortho_projection.h"
#include "../component_tag.h"
#include "component.h"
#include "glm/ext/matrix_clip_space.hpp"

bool camera::system::camera_orthographic_projection_system::update() noexcept {
  using namespace component;

  const auto& view = registry.view<camera_setting_component, entt::tag<component_tag::orthographic_projection>, set_projection_request>();

  for (auto&& [entity, settings]: view.each()) {
    const auto& half_width = static_cast<float>(settings.width) / 2;
    const auto& half_height = static_cast<float>(settings.height) / 2;
    const auto& projection = glm::ortho(-half_width, half_width, -half_height, half_height, settings.near, settings.far);

    registry.emplace_or_replace<projection_component>(entity, projection);
    registry.remove<set_projection_request>(entity);

    if (registry.all_of<entt::tag<component_tag::perspective_projection>>(entity)) registry.remove<entt::tag<component_tag::perspective_projection>>(entity);
  }

  return true;
}