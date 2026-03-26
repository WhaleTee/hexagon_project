#pragma once
#include "game_event.h"

namespace event {
  struct game_quit_event final : game_event<game_quit_event> {};
} // namespace event
