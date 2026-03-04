#pragma once
#include "../components.h"
#include <entt/entity/registry.hpp>

namespace ecs::systems {
  class base_system {
  protected:
    entt::registry& registry;
    entt::dispatcher& dispatcher;

  public:
    explicit base_system(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {}

    virtual ~base_system() noexcept = default;

    virtual bool update() noexcept = 0;
  };

  using system = base_system;
} // namespace ecs::systems
