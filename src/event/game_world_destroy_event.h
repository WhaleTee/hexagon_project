#pragma once
#include "dispatcher_invoker.h"

namespace event {
  struct game_world_destroy_event {
    static constexpr dispatcher_invoker<game_world_destroy_event> invoker{};
    game_world_destroy_event() noexcept = default;
  };
} // namespace event
