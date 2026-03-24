#pragma once
#include "SDL3/SDL_init.h"
#include "ecs/game_world.h"

class sdl_app_runner {
  std::unique_ptr<ecs::game_world> world;
  bool app_state{false};

public:
  sdl_app_runner() noexcept;

  SDL_AppResult start(int argc, char** argv) noexcept;

  SDL_AppResult update() noexcept;

  SDL_AppResult handle_event(SDL_Event* event) noexcept;

  void quit() noexcept;

  void set_app_state_exit() noexcept;
};