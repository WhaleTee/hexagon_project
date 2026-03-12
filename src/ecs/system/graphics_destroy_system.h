#pragma once
#include "../../event/game_world_destroy_event.h"
#include "SDL3/SDL_gpu.h"
#include "base_system.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

namespace ecs::system {
  class graphics_destroy_system final : public system {
  public:
    graphics_destroy_system(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : system(registry, dispatcher) {}

    ~graphics_destroy_system() override = default;

    bool update() noexcept override {
      using namespace ecs::component;

      const auto view = registry.view<gpu_device_component, window_component, graphics_destroy_request_tag>();
      if (view.begin() == view.end()) return false;
      for (const auto& [entity, device, window]: view.each()) {
        SDL_ReleaseWindowFromGPUDevice(device.value_ptr, window.value_ptr);
        SDL_DestroyWindow(window.value_ptr);
        SDL_DestroyGPUDevice(device.value_ptr);
      }

      dispatcher.trigger<event::game_world_destroy_event>();
      mark_to_destroy();
      return true;
    }
  };
} // namespace ecs::system