#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include "SDL3/SDL_log.h"

#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct create_gpu_device final : ecs::system::system {
    explicit create_gpu_device(entt::registry& registry) : base_system(registry) {}

    ~create_gpu_device() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto gpu_device_setting_view = registry.view<gpu_device_setting_component>();
      const auto entity = gpu_device_setting_view.front();
      const auto [shader_format, driver_name, debug] = gpu_device_setting_view.get<const gpu_device_setting_component>(entity);
      auto* gpu_device = SDL_CreateGPUDevice(shader_format, debug, driver_name);

      if (!gpu_device) {
        SDL_Log("Failed to create gpu device: %s", SDL_GetError());
        return false;
      }

      registry.emplace<gpu_device_component>(entity, gpu_device);

      mark_to_destroy();
      return true;
    }
  };

  struct destroy_gpu_device final : ecs::system::system {
    explicit destroy_gpu_device(entt::registry& registry) : base_system(registry) {}

    ~destroy_gpu_device() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto gpu_device_destroy_request_view = registry.view<gpu_device_component, gpu_device_setting_component, gpu_device_destroy_request>();
      const auto entity = gpu_device_destroy_request_view.front();

      if (!registry.all_of<gpu_device_destroy_request>(entity)) return true;

      const auto gpu_device_view = registry.view<gpu_device_component>();
      auto* gpu_device = gpu_device_view.get<gpu_device_component>(entity).value;

      SDL_DestroyGPUDevice(gpu_device);

      registry.remove<gpu_device_component>(entity);
      registry.remove<gpu_device_setting_component>(entity);
      registry.remove<gpu_device_destroy_request>(entity);

      mark_to_destroy();
      return true;
    }
  };
} // namespace rendering::system