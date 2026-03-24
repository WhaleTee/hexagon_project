#include "../event/game_world_destroy_event.h"
#include "../sdl_app_runner.h"
#include "../example/example_game_world.h"

sdl_app_runner::sdl_app_runner() noexcept {
  world = std::make_unique<example_game_world>();
}

SDL_AppResult sdl_app_runner::start(int argc, char** argv) noexcept {
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO) || !SDL_InitSubSystem(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

  world->register_event_listener<event::game_world_destroy_event, &sdl_app_runner::set_app_state_exit>(*this);
  world->run();

  app_state = true;

  return SDL_APP_CONTINUE;
}

SDL_AppResult sdl_app_runner::update() noexcept {
  if (!app_state) return SDL_APP_SUCCESS;

  world->update();

  return SDL_APP_CONTINUE;
}

SDL_AppResult sdl_app_runner::handle_event(SDL_Event* event) noexcept {
  if (!app_state) return SDL_APP_SUCCESS;

  world->dispatch_event(event);

  return SDL_APP_CONTINUE;
}

void sdl_app_runner::quit() noexcept {
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
  SDL_Quit();
}

void sdl_app_runner::set_app_state_exit() noexcept {
  app_state = false;
}