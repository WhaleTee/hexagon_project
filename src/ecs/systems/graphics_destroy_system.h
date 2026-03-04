#pragma once
#include "SDL3/SDL_gpu.h"
#include "base_system.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <string>

namespace ecs::systems {
  class graphics_destroy_system final : public system {
  public:
    graphics_destroy_system(entt::registry& registry, entt::dispatcher& dispatcher) : system(registry, dispatcher) {}

    ~graphics_destroy_system() override = default;

    bool update() noexcept override {
      using namespace ecs::components;

      const auto view = registry.view<gpu_device_component, window_component, graphics_destroy_request_tag>().each();
      if (view.begin() == view.end()) return false;
      for (auto [entity, device, window]: view) {
        SDL_ReleaseWindowFromGPUDevice(device.value_ptr, window.value_ptr);
        SDL_DestroyWindow(window.value_ptr);
        SDL_DestroyGPUDevice(device.value_ptr);
      }

      mark_to_destroy();
      return true;
    }

    static constexpr std::string get_class_name() noexcept {
      return "graphics_destroy_system";
    }
  };
} // namespace ecs::systems