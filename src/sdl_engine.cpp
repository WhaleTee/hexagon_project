#define SDL_MAIN_USE_CALLBACKS

#include "sdl_app_runner.h"
#include <SDL3/SDL_main.h>

namespace {
  sdl_app_runner app_runner{};
} // namespace


SDL_AppResult SDL_AppInit(void** appState, int argc, char** argv) {
  return app_runner.start(argc, argv);
}

SDL_AppResult SDL_AppIterate(void* appState) {
  return app_runner.update();
}

SDL_AppResult SDL_AppEvent(void* appState, SDL_Event* event) {
  return app_runner.handle_event(event);
}

void SDL_AppQuit(void* appState, SDL_AppResult result) {
  return app_runner.quit();
}