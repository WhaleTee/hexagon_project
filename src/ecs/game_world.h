#pragma once
#include "SDL3/SDL_events.h"
#include "system_manager.h"
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

namespace ecs {
  class game_world {
  protected:
    entt::registry registry{};
    entt::dispatcher dispatcher{};
    system_manager system_manager{registry, dispatcher};

  public:
    virtual ~game_world() = default;

    void run() noexcept;

    void update() noexcept;

    template<typename T, auto Candidate, typename I>
    void register_event_listener(I& value_or_instance) noexcept {
      this->dispatcher.sink<T>().template connect<Candidate>(value_or_instance);
    }

    template<typename T, auto Candidate>
    void register_event_listener() noexcept {
      this->dispatcher.sink<T>().template connect<Candidate>();
    }

    virtual void dispatch_event(SDL_Event* event) noexcept {}
  };
} // namespace ecs
