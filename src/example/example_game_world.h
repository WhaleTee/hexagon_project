#pragma once
#include "../ecs/game_world.h"

class example_game_world final : public ecs::game_world {
public:
  example_game_world() noexcept;
  void dispatch_event(SDL_Event* event) noexcept override;
};
