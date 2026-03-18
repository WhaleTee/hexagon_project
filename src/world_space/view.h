#pragma once
#include "../camera/component.h"
#include "../ecs/base_system.h"
#include "../rendering/render.h"
#include "glm/gtc/quaternion.hpp"
#include <entt/entity/registry.hpp>

namespace space::system {
  struct view_matrix_system final : ecs::system::base_system {
    explicit view_matrix_system(entt::registry& registry) : base_system(registry) {}

    ~view_matrix_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace camera::component;

      auto view = registry.view<orientation_component, position_component, view_matrix_component>();

      for (auto [entity, orientation, position, view_matrix]: view.each()) {
        auto rotation_matrix = glm::mat4_cast(orientation.value);
        auto translation_matrix = glm::translate(glm::mat4(1.f), -position.value);
        view_matrix.value = rotation_matrix * translation_matrix;
      }

      return true;
    }
  };
} // namespace space