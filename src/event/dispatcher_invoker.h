#pragma once
#include <entt/signal/dispatcher.hpp>

namespace event {
  template <typename T, typename... Args>
  struct dispatcher_invoker {
    constexpr dispatcher_invoker() noexcept = default;

    void operator()(entt::dispatcher& dispatcher, Args&&... args) noexcept {
      dispatcher.trigger<T>(std::forward<Args>(args)...);
    }
  };
} // namespace event