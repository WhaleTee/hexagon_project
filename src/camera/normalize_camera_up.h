#pragma once
#include "../ecs/base_system.h"
#include "../world_space/component.h"
#include "component.h"

namespace camera::system {
  struct normalize_camera_up_system final : ecs::system::base_system {
    explicit normalize_camera_up_system(entt::registry& registry) : base_system(registry) {}

    ~normalize_camera_up_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace space::component;

      const auto& view = registry.view<view_matrix_component, orientation_component, position_component>();

      for (auto&& [entity, matrix, orientation, position] : view.each()) {
        auto forward = glm::vec3{matrix.value[0][2], matrix.value[1][2], matrix.value[2][2]};
        orientation.value = glm::lookAtLH(position.value, position.value + forward, glm::vec3{0, 1.f, 0});
      }

      return true;
    }
  };
}