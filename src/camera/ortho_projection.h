#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace camera::system {
  class camera_orthographic_projection_system final : public ecs::system::base_system {
  public:
    explicit camera_orthographic_projection_system(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace camera::system