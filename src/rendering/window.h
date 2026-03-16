#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include "SDL3/SDL_log.h"

#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct create_window final : ecs::system::system {
    explicit create_window(entt::registry& registry) : base_system(registry) {}

    ~create_window() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto window_setting_view = registry.view<window_setting_component>();
      const auto entity = window_setting_view.front();
      const auto [window_flags, name, width, height] = window_setting_view.get<const window_setting_component>(entity);
      auto* window = SDL_CreateWindow(name, static_cast<int>(width), static_cast<int>(height), window_flags);

      if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
      }

      registry.emplace<window_component>(entity, window);

      mark_to_destroy();
      return true;
    }
  };

  struct destroy_window final : ecs::system::system {
    explicit destroy_window(entt::registry& registry) : base_system(registry) {}

    ~destroy_window() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto window_destroy_request_view = registry.view<window_component, window_setting_component, window_destroy_request>();
      const auto entity = window_destroy_request_view.front();

      if (!registry.all_of<window_destroy_request>(entity)) return true;

      const auto window_view = registry.view<window_component>();
      auto* window = window_view.get<window_component>(entity).value;

      SDL_DestroyWindow(window);

      registry.remove<window_component>(entity);
      registry.remove<window_setting_component>(entity);
      registry.remove<window_destroy_request>(entity);

      mark_to_destroy();
      return true;
    }
  };
} // namespace rendering::system