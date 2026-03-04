#include "world.h"
#include "../event/game_start_event.h"

namespace ecs {
  world::world() noexcept {
    dispatcher.sink<::event::game_quit_event>().connect<&world::on_game_quit_event>(*this);
  }

  world::~world() noexcept {}

  world_state world::run() noexcept {
    if (!system_manager.get_systems_count()) return world_state::uninitialized;
    this->dispatcher.trigger<::event::game_start_event>();
    this->running = true;
    this->system_manager.start();
    return world_state::running;
  }

  void world::update() noexcept {
    if (running) this->system_manager.update();
  }

  void world::dispatch_event(SDL_Event* event) noexcept {
    switch (event->type) {
    case SDL_EVENT_QUIT:
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      dispatcher.trigger<::event::game_quit_event>();
      break;

    default:
      break;
    }
  }

  void world::on_game_quit_event(const ::event::game_quit_event event) noexcept {
    this->running = false;
  }
} // namespace ecs