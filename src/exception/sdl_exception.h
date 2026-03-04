#pragma once

#include "SDL3/SDL.h"
#include <stdexcept>

namespace exception {
  class sdl_exception : std::runtime_error {
  public:
    explicit sdl_exception(const std::string& msg) : std::runtime_error(msg + '\n' + SDL_GetError()) {}
  };
}