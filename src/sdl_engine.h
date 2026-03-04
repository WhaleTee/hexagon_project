#pragma once

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appState, int argc, char **argv);
SDL_AppResult SDL_AppIterate(void *appState);
SDL_AppResult SDL_AppEvent(void *appState, SDL_Event *event);
void SDL_AppQuit(void *appState, SDL_AppResult result);