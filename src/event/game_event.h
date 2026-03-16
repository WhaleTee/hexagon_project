#pragma once
#include "dispatcher_invoker.h"

namespace event {
  template <typename T> struct game_event {
    static constexpr dispatcher_invoker<T> invoker{};
  };
} // namespace event