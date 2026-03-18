#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"

#include <entt/entity/registry.hpp>
#include <iostream>

namespace rendering::system {
  struct begin_render_pass final : ecs::system::base_system {
    explicit begin_render_pass(entt::registry& registry) : base_system(registry) {}

    ~begin_render_pass() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto& render_cmd_buffer_view = registry.view<gpu_command_buffer_component, entt::tag<component_tag::render>>();
      const auto& render_cmd_buffer_entity = render_cmd_buffer_view.front();

      if (!registry.valid(render_cmd_buffer_entity)) {
        SDL_Log("Cannot begin render pass. Render command buffer not exist.");
        return false;
      }

      const auto& command_buffer = render_cmd_buffer_view.get<gpu_command_buffer_component>(render_cmd_buffer_entity);
      const auto& window_view = registry.view<window_component, window_setting_component, gpu_color_target_setting_component>();
      const auto& window_entity = window_view.front();

      if (!registry.valid(window_entity)) {
        SDL_Log("Cannot begin render pass. Render command buffer not exist.");
        return false;
      }

      auto [window, window_setting, color_target_setting] = window_view.get<window_component, window_setting_component, gpu_color_target_setting_component>(window_entity);

      color_target_setting.swapchainTexture = nullptr;
      if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer.value, window.value, &color_target_setting.swapchainTexture, nullptr, nullptr)) {
        SDL_Log("Acquire swapchain texture failed: %s", SDL_GetError());
        return false;
      }

      if (color_target_setting.swapchainTexture != nullptr) {
        SDL_GPUColorTargetInfo color_target_info{};
        color_target_info.texture = color_target_setting.swapchainTexture;
        color_target_info.clear_color = color_target_setting.clear_color;
        color_target_info.load_op = color_target_setting.load_op;
        color_target_info.store_op = color_target_setting.store_op;

        registry.emplace_or_replace<gpu_render_pass_component>(render_cmd_buffer_entity, SDL_BeginGPURenderPass(command_buffer.value, &color_target_info, 1, nullptr));
      }

      return true;
    }
  };

  struct end_render_pass final : ecs::system::base_system {
    explicit end_render_pass(entt::registry& registry) : base_system(registry) {}

    ~end_render_pass() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto render_pass_view = registry.view<gpu_render_pass_component>();
      const auto render_pass_entity = render_pass_view.front();

      if (registry.valid(render_pass_entity)) {
        SDL_EndGPURenderPass(render_pass_view.get<gpu_render_pass_component>(render_pass_entity).value);

        registry.remove<gpu_render_pass_component>(render_pass_entity);
      }

      return true;
    }
  };
} // namespace rendering::system