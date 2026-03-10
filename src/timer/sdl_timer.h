#pragma once
#include "SDL3/SDL_stdinc.h"

namespace timer {
  class sdl_timer {
    bool started{false};
    bool paused{false};
    unsigned long long start_ticks{0};
    unsigned long long pause_ticks{0};
    int rate{0};

  public:
    void start() noexcept;
    void stop() noexcept;
    void pause() noexcept;
    void set_rate(int) noexcept;
    [[nodiscard]] bool is_started() const noexcept;
    [[nodiscard]] bool is_paused() const noexcept;
    [[nodiscard]] Uint64 get_ticks() const noexcept;
    [[nodiscard]] Uint64 get_fixed_ticks() const noexcept;
    [[nodiscard]] Uint64 get_delay() const noexcept;
  };
} // namespace timer