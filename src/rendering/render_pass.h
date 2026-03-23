#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class begin_render_pass final : public ecs::system::base_system {
  public:
    explicit begin_render_pass(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

  class end_render_pass final : public ecs::system::base_system {
  public:
    explicit end_render_pass(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace rendering::system