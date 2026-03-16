#pragma once
#include "SDL3/SDL_events.h"

class sdl_game_app {
  public:
  void update();
  void render();
  void handle_event(SDL_Event event);
};