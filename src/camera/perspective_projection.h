#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace camera::system {
  class camera_perspective_projection_system final : public ecs::system::base_system {
  public:
    explicit camera_perspective_projection_system(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
}