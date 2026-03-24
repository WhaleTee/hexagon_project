#pragma once
#include "../event/game_quit_event.h"
#include "base_system.h"
#include <entt/entity/registry.hpp>
#include <memory>

namespace ecs {
  class system_manager {
    using system_ptr = std::unique_ptr<system::base_system>;
    using system_registry = std::vector<system_ptr>;

    entt::registry& registry;
    entt::dispatcher& dispatcher;
    system_registry systems{};
    bool destroy_all{false};

    system::base_system& add_system(system_ptr&& system) noexcept;

    void update_systems() noexcept;

  public:
    explicit system_manager(entt::registry& registry, entt::dispatcher& dispatcher) noexcept;

    ~system_manager() noexcept = default;

    void update() noexcept;

    template <typename T>
    T& create_system() noexcept {
      return static_cast<T&>(add_system(std::move(std::make_unique<T>(this->registry))));
    }

    template <typename T, typename... TArgs>
    T& create_system(TArgs&&... args) noexcept {
      return static_cast<T&>(add_system(std::move(std::make_unique<T>(this->registry, std::forward<TArgs>(args)...))));
    }

    [[nodiscard]] bool has_systems() const noexcept;

    void handle_game_quit_event(event::game_quit_event event) noexcept;
  };
} // namespace ecs