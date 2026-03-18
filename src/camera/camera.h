#pragma once
#include "../component_tag.h"
#include "../ecs/base_system.h"
#include "../world_space/rotation.h"
#include "component.h"
#include "glm/gtc/quaternion.hpp"
#include <entt/entity/registry.hpp>

namespace camera::system {
  struct camera_orthographic_projection_system final : ecs::system::base_system {
    explicit camera_orthographic_projection_system(entt::registry& registry) : base_system(registry) {}

    ~camera_orthographic_projection_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      auto view = registry.view<projection_component, camera_setting_component, entt::tag<component_tag::orthographic>>();

      for (auto [entity, projection, settings]: view.each()) {
        const float half_width = static_cast<float>(settings.width) / 2.f;
        const float half_height = static_cast<float>(settings.height) / 2.f;
        projection.value = glm::ortho(-half_width, half_width, -half_height, half_height, settings.near, settings.far);
      }

      return true;
    }
  };

  struct camera_perspective_projection_system final : ecs::system::base_system {
    explicit camera_perspective_projection_system(entt::registry& registry) : base_system(registry) {}

    ~camera_perspective_projection_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace space::component;

      auto view = registry.view<projection_component, camera_setting_component, field_of_view_component, entt::tag<component_tag::perspective>>();

      for (auto [entity, projection, settings, fov]: view.each()) {
        const auto width = static_cast<float>(settings.width);
        const auto height = static_cast<float>(settings.height);
        projection.value = glm::perspective(glm::radians(fov.value), width / height, settings.near, settings.far);
      }

      return true;
    }
  };
} // namespace camera::system