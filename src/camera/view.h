#pragma once
#include "../ecs/base_system.h"
#include "../rendering/render.h"
#include "glm/gtc/quaternion.hpp"
#include <entt/entity/registry.hpp>

namespace transform::system {
  struct view_matrix_system final : ecs::system::base_system {
    explicit view_matrix_system(entt::registry& registry) : base_system(registry) {}

    ~view_matrix_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace camera::component;

      for (auto&& [entity, rotation, position, view_matrix]: registry.view<rotation_component, position_component, view_matrix_component>().each()) {
        const auto rotation_matrix = glm::mat4_cast(rotation.value);
        const auto position_matrix = glm::translate(glm::mat4{1}, -position.value);

        view_matrix.value = rotation_matrix * position_matrix;
      }

      return true;
    }
  };
} // namespace space::system