#pragma once
#include "dispatcher_invoker.h"

namespace event {
  struct game_quit_event {
    static constexpr dispatcher_invoker<game_quit_event> invoker{};
    game_quit_event() noexcept = default;
  };
} // namespace event
