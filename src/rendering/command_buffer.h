#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  template <std::uint32_t Tag> struct initialize_command_buffer final : ecs::system::base_system {
    explicit initialize_command_buffer(entt::registry& registry) : base_system(registry) {}

    ~initialize_command_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto gpu_device_view = registry.view<gpu_device_component>();

      auto gpu_device_entity = gpu_device_view.front();

      if (!registry.valid(gpu_device_entity)) { SDL_Log("Cannot initialize command buffer: tag [%s]. GPU device now found.", Tag); }

      auto* gpu_device = gpu_device_view.template get<gpu_device_component>(gpu_device_view.front()).value;

      auto cmd_buffer_entity = registry.create();
      registry.emplace<gpu_command_buffer_component>(cmd_buffer_entity, SDL_AcquireGPUCommandBuffer(gpu_device));
      registry.insert<entt::tag<Tag>>(cmd_buffer_entity);

      return true;
    }
  };

  template <std::uint32_t Tag> struct submit_copy_command_buffer final : ecs::system::base_system {
    explicit submit_copy_command_buffer(entt::registry& registry) : base_system(registry) {}

    ~submit_copy_command_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto copy_cmd_buffer_view = registry.view<gpu_command_buffer_component, entt::tag<Tag>>();
      const auto copy_cmd_buffer_entity = copy_cmd_buffer_view.front();

      if (registry.valid(copy_cmd_buffer_entity)) {
        auto* copy_cmd_buffer = copy_cmd_buffer_view.template get<gpu_command_buffer_component>(copy_cmd_buffer_entity).value;

        SDL_SubmitGPUCommandBuffer(copy_cmd_buffer);

        registry.remove<gpu_command_buffer_component>(copy_cmd_buffer_entity);
      }


      return true;
    }
  };
} // namespace rendering::system