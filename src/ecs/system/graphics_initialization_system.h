#pragma once
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"
#include "base_system.h"

namespace ecs::system {
  class graphics_initialization_system final : public system {
    bool initialized{false};

  public:
    graphics_initialization_system(entt::registry& registry, entt::dispatcher& dispatcher) : system(registry, dispatcher) {}

    ~graphics_initialization_system() override = default;

    bool update() noexcept override {
      using namespace ecs::component;

      const auto view = registry.view<gpu_device_setting_component, window_setting_component>().each();
      for (auto [entity, gpu_settings, window_settings]: view) {
        if (!initialized) {
          auto& [device] = registry.emplace<gpu_device_component>(entity);
          auto& [window] = registry.emplace<window_component>(entity);
          device = SDL_CreateGPUDevice(gpu_settings.shader_format, gpu_settings.debug_mode, gpu_settings.driver_name);
          window = SDL_CreateWindow(window_settings.name, window_settings.width, window_settings.height, window_settings.window_flags);
          if (!device || !window) return false;
          if (!SDL_ClaimWindowForGPUDevice(device, window)) return false;
          registry.erase<gpu_device_setting_component, window_setting_component>(entity);
          initialized = true;
        } else {
          if (!registry.any_of<gpu_device_component, window_component>(entity)) registry.destroy(entity);
        }
      }

      mark_to_destroy();
      return true;
    }
  };
} // namespace ecs::systems