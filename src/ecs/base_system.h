#pragma once
#include <entt/entity/registry.hpp>

namespace ecs::system {
  class base_system {
    bool marked_to_destroy{false};

  protected:
    entt::registry& registry;

  public:
    explicit base_system(entt::registry& registry) noexcept : registry(registry) {}

    virtual ~base_system() noexcept = default;

    virtual bool update() noexcept = 0;

    [[nodiscard]] bool is_marked_to_destroy() const noexcept {
      return marked_to_destroy;
    }

    void mark_to_destroy() noexcept {
      marked_to_destroy = true;
    }
  };
} // namespace ecs::system
