#pragma once
#include "../timer/sdl_timer.h"
#include "systems/base_system.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <memory>
#include <queue>
#include <vector>

namespace ecs {
  class system_manager {
    using system_ptr = std::unique_ptr<systems::system>;
    using system_registry = std::vector<system_ptr>;
    using system_queue = std::queue<system_ptr>;

    entt::registry& registry;
    entt::dispatcher& dispatcher;
    system_queue add_system_requests;
    system_registry systems{};
    timer::sdl_timer timer{};
    bool game_is_running{false};
    bool marked_to_sweep{false};

    systems::system& add_system_internal(system_ptr&& system) noexcept;

  public:
    explicit system_manager(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {}

    ~system_manager() noexcept = default;

    void start() noexcept;

    void update() noexcept;

    void update_systems() noexcept;

    template <typename T, typename... TArgs>
    T& create_system(TArgs&&... args) noexcept;
  };
}

#include "system_manager.ipp"