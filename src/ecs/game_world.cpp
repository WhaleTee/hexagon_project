#include "../event/game_start_event.h"
#include "../event/game_world_destroy_event.h"
#include "game_world.h"

namespace ecs {
  void game_world::run() noexcept {
    this->dispatcher.trigger<event::game_start_event>();
  }

  void game_world::update() noexcept {
    if (this->system_manager.has_systems()) this->system_manager.update();
    else event::game_world_destroy_event::invoker(this->dispatcher);
  }
} // namespace ecs