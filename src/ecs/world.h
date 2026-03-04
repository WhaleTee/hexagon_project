#pragma once
#include "../event/game_quit_event.h"
#include "SDL3/SDL_events.h"
#include "system_manager.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

namespace ecs {
  enum world_state {
    uninitialized,
    running,
    error,
  };

  class world {
    bool running{false};

  protected:
    entt::registry registry{};
    entt::dispatcher dispatcher{};
    system_manager system_manager{registry, dispatcher};

  public:
    explicit world() noexcept;

    virtual ~world() noexcept;

    world_state run() noexcept;

    void update() noexcept;

    virtual void dispatch_event(SDL_Event* event) noexcept;

    void on_game_quit_event(::event::game_quit_event event) noexcept;
  };
} // namespace ecs
