#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"

#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct begin_copy_pass final : ecs::system::base_system {
    explicit begin_copy_pass(entt::registry& registry) : base_system(registry) {}

    ~begin_copy_pass() noexcept override = default;

    bool update() noexcept override {
      mark_to_destroy();

      using namespace component;

      const auto copy_cmd_buffer_view = registry.view<gpu_command_buffer_component>();
      const auto copy_cmd_buffer_entity = copy_cmd_buffer_view.front();

      if (!registry.valid(copy_cmd_buffer_entity)) {
        SDL_Log("Cannot begin copy pass. Command buffer was not created.");
        return false;
      }

      auto* command_buffer = copy_cmd_buffer_view.get<gpu_command_buffer_component>(copy_cmd_buffer_entity).value;

      registry.emplace<gpu_copy_pass_component>(copy_cmd_buffer_entity, SDL_BeginGPUCopyPass(command_buffer));

      return true;
    }
  };

  struct end_copy_pass final : ecs::system::base_system {
    explicit end_copy_pass(entt::registry& registry) : base_system(registry) {}

    ~end_copy_pass() noexcept override = default;

    bool update() noexcept override {
      mark_to_destroy();

      using namespace component;

      const auto copy_pass_view = registry.view<gpu_copy_pass_component>();
      const auto copy_pass_entity = copy_pass_view.front();

      if (!registry.valid(copy_pass_entity)) {
        SDL_EndGPUCopyPass(copy_pass_view.get<gpu_copy_pass_component>(copy_pass_entity).value);

        registry.remove<gpu_copy_pass_component>(copy_pass_entity);
      }

      return true;
    }
  };

} // namespace rendering::system