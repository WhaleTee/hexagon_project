#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class render_mvp_system final : public ecs::system::base_system {
  public:
    explicit render_mvp_system(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace rendering::system