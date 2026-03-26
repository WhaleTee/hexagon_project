#include "game_world.h"
#include "../event/game_start_event.h"
#include "../event/game_world_destroy_event.h"

namespace ecs {
  void game_world::run() noexcept {
    event::game_start_event::invoker(this->dispatcher);
    register_event_listener<event::game_quit_event, &system_manager::destroy_systems>(system_manager);
  }

  void game_world::update() noexcept {
    if (this->system_manager.has_systems()) this->system_manager.update();
    else event::game_world_destroy_event::invoker(this->dispatcher);
  }
} // namespace ecs