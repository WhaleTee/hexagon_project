#pragma once
#include "../component.h"
#include <entt/entity/registry.hpp>

namespace ecs::system {

  class base_system {
    bool marked_to_destroy{false};
  protected:
    entt::registry& registry;
    entt::dispatcher& dispatcher;

  public:
    explicit base_system(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {}

    virtual ~base_system() noexcept = default;

    virtual bool update() noexcept = 0;

    [[nodiscard]] bool is_marked_to_destroy() const noexcept { return marked_to_destroy; }

    void mark_to_destroy() noexcept { marked_to_destroy = true; }
  };

  using system = base_system;
} // namespace ecs::systems
