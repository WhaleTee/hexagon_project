#pragma once
#include "game_event.h"

namespace event {
  struct game_world_destroy_event final : game_event<game_world_destroy_event> {};
} // namespace event
