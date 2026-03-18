#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct create_window final : ecs::system::base_system {
    explicit create_window(entt::registry& registry) : base_system(registry) {}

    ~create_window() noexcept override = default;

    bool update() noexcept override {
      mark_to_destroy();

      using namespace component;

      const auto& window_setting_view = registry.view<window_setting_component, gpu_device_setting_component>();
      const auto& window_entity = window_setting_view.front();
      const auto [window_flags, name, width, height] = window_setting_view.get<const window_setting_component>(window_entity);
      auto* window = SDL_CreateWindow(name, static_cast<int>(width), static_cast<int>(height), window_flags);

      if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
      }
      const auto [shader_format, driver_name, debug] = window_setting_view.get<const gpu_device_setting_component>(window_entity);
      auto* gpu_device = SDL_CreateGPUDevice(shader_format, debug, driver_name);

      if (!gpu_device) {
        SDL_Log("Failed to create gpu device: %s", SDL_GetError());
        return false;
      }

      if (!SDL_ClaimWindowForGPUDevice(gpu_device, window)) {
        SDL_Log("Claim window for GPU device failed: %s", SDL_GetError());
        return false;
      }

      registry.emplace<window_component>(window_entity, window);
      registry.emplace<gpu_device_component>(window_entity, gpu_device);

      return true;
    }
  };

  struct destroy_window final : ecs::system::base_system {
    entt::dispatcher& dispatcher;
    explicit destroy_window(entt::registry& registry, entt::dispatcher& dispatcher) : base_system(registry), dispatcher(dispatcher) {}

    ~destroy_window() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto& destroy_request_entity = registry.view<window_component, window_destroy_request>().front();

      if (!registry.valid(destroy_request_entity)) return true;

      mark_to_destroy();

      const auto& window_view = registry.view<window_component, gpu_device_component>();
      const auto& window_entity = window_view.front();
      auto* window = window_view.get<window_component>(window_entity).value;
      auto* gpu_device = window_view.get<gpu_device_component>(window_entity).value;

      SDL_ReleaseWindowFromGPUDevice(gpu_device, window);
      SDL_DestroyGPUDevice(gpu_device);
      SDL_DestroyWindow(window);

      registry.remove<window_component, gpu_device_component, window_destroy_request>(destroy_request_entity);

      event::game_world_destroy_event::invoker(dispatcher);

      return true;
    }
  };
} // namespace rendering::system