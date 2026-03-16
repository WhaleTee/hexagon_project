#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_copy_pass final : ecs::system::system {
    explicit initialize_copy_pass(entt::registry& registry) : base_system(registry) {}

    ~initialize_copy_pass() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto copy_cmd_buffer_view = registry.view<gpu_copy_command_buffer_component>();
      const auto entity = copy_cmd_buffer_view.front();
      auto* command_buffer = copy_cmd_buffer_view.get<gpu_copy_command_buffer_component>(entity).value;

      registry.emplace<gpu_copy_pass_component>(entity, SDL_BeginGPUCopyPass(command_buffer));
      mark_to_destroy();
      return true;
    }
  };

  struct destroy_copy_pass final : ecs::system::system {
    explicit destroy_copy_pass(entt::registry& registry) : base_system(registry) {}

    ~destroy_copy_pass() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto copy_pass_view = registry.view<gpu_copy_pass_component>();
      const auto entity = copy_pass_view.front();

      SDL_EndGPUCopyPass(copy_pass_view.get<gpu_copy_pass_component>(entity).value);

      registry.emplace<gpu_copy_pass_component>(entity);
      mark_to_destroy();
      return true;
    }
  };
} // namespace rendering::system