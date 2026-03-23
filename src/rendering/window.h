#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class create_window final : public ecs::system::base_system {
  public:
    explicit create_window(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

  class destroy_window final : public ecs::system::base_system {
    entt::dispatcher& dispatcher;

  public:
    explicit destroy_window(entt::registry& registry, entt::dispatcher& dispatcher) : base_system(registry), dispatcher(dispatcher) {}

    bool update() noexcept override;
  };
} // namespace rendering::system