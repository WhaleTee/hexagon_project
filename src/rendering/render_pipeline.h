#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class initialize_render_pipeline final : public ecs::system::base_system {
  public:
    explicit initialize_render_pipeline(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

  class release_render_pipeline final : public ecs::system::base_system {
  public:
    explicit release_render_pipeline(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace rendering::system