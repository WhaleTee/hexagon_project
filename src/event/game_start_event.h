#pragma once
#include "game_event.h"

namespace event {
  struct game_start_event final : game_event<game_start_event> {};
} // namespace event
