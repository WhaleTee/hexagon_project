#include "sdl_timer.h"
#include "SDL3/SDL_timer.h"

namespace timer {
  void sdl_timer::start() noexcept {
    started = true;
    paused = false;
    start_ticks = SDL_GetTicks();
  }

  void sdl_timer::stop() noexcept {
    started = false;
    paused = false;
    start_ticks = 0;
    pause_ticks = 0;
  }

  void sdl_timer::pause() noexcept {
    if (started && !paused) {
      paused = true;
      pause_ticks = SDL_GetTicks() - start_ticks;
      start_ticks = 0;
    }
  }

  void sdl_timer::unpause() noexcept {
    if (started && paused) {
      paused = false;
      start_ticks = SDL_GetTicks() - pause_ticks;
      pause_ticks = 0;
    }
  }

  void sdl_timer::set_rate(const int value) noexcept {
    if (value <= 0) return;
    rate = value;
  }

  [[nodiscard]] bool sdl_timer::is_started() const noexcept {
    return started;
  }

  [[nodiscard]] bool sdl_timer::is_paused() const noexcept {
    return started && paused;
  }

  [[nodiscard]] Uint64 sdl_timer::get_ticks() const noexcept {
    Uint64 ticks = 0;
    if (started) {
      if (paused) ticks = pause_ticks;
      else ticks = SDL_GetTicks() - start_ticks;
    }
    return ticks;
  }

  [[nodiscard]] Uint64 sdl_timer::get_fixed_ticks() const noexcept {
    return 1000 / rate;
  }

  [[nodiscard]] Uint64 sdl_timer::get_delay_to_fixed_ticks() const noexcept {
    Uint64 delay{0};
    if (rate > 0) {
      const Uint64 rate_ms = get_fixed_ticks();
      if (const Uint64 delta = get_ticks(); delta < rate_ms) { delay = rate_ms - delta; }
    }
    return delay;
  }
} // namespace timer