#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_render_command_buffer final : ecs::system::system {
    explicit initialize_render_command_buffer(entt::registry& registry) : base_system(registry) {}

    ~initialize_render_command_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto device_view = registry.view<gpu_device_component>();
      auto* device = device_view.get<gpu_device_component>(device_view.front()).value;

      registry.emplace<gpu_render_command_buffer_component>(registry.create(), SDL_AcquireGPUCommandBuffer(device));
      mark_to_destroy();
      return true;
    }
  };

  struct submit_render_command_buffer final : ecs::system::system {
    explicit submit_render_command_buffer(entt::registry& registry) : base_system(registry) {}

    ~submit_render_command_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto copy_cmd_buffer_view = registry.view<gpu_render_command_buffer_component>();
      const auto entity = copy_cmd_buffer_view.front();
      auto* copy_cmd_buffer = copy_cmd_buffer_view.get<gpu_render_command_buffer_component>(entity).value;

      SDL_SubmitGPUCommandBuffer(copy_cmd_buffer);

      registry.remove<gpu_render_command_buffer_component>(entity);
      mark_to_destroy();
      return true;
    }
  };
} // namespace rendering::system