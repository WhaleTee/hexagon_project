#pragma once
#include "../ecs/base_system.h"
#include "../rendering/render.h"
#include "component.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include <entt/entity/registry.hpp>

namespace space::system {
  struct rotation_system final : ecs::system::base_system {
    explicit rotation_system(entt::registry& registry) : base_system(registry) {}

    ~rotation_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      auto view = registry.view<orientation_component, rotation_request>();

      for (auto [entity, orientation, request]: view.each()) {
        auto rotation = glm::angleAxis(request.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotation *= glm::angleAxis(request.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation *= glm::angleAxis(request.z, glm::vec3(0.0f, 0.0f, 1.0f));
        orientation.value = glm::normalize(orientation.value * rotation);
        registry.remove<rotation_request>(entity);
      }

      return true;
    }
  };
} // namespace space