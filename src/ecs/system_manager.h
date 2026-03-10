#pragma once
#include "system/base_system.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <memory>

namespace ecs {
  class system_manager {
    using system_ptr = std::unique_ptr<system::system>;
    using system_registry = std::vector<system_ptr>;

    entt::registry& registry;
    entt::dispatcher& dispatcher;
    system_registry systems{};

    system::system& add_system(system_ptr&& system) noexcept;

    void update_systems() noexcept;

  public:
    explicit system_manager(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {}

    ~system_manager() noexcept = default;

    void update() noexcept;

    template <typename T, typename... TArgs>
    T& create_system(TArgs&&... args) noexcept {
      return static_cast<T&>(add_system(std::move(std::make_unique<T>(this->registry, std::forward<TArgs>(args)...))));
    }

    [[nodiscard]] bool has_systems() const noexcept {
      return !systems.empty();
    }
  };
} // namespace ecs