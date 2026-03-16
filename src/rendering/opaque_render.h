#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct render_opaque_system final : ecs::system::base_system {
    explicit render_opaque_system(entt::registry& registry) : base_system(registry) {}

    ~render_opaque_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto render_pipeline_view = registry.view<opaque_render_pipeline_component>();
      const auto render_pipeline_entity = render_pipeline_view.front();

      if (registry.valid(render_pipeline_entity)) {
        const auto render_pass_view = registry.view<gpu_render_pass_component>();
        const auto render_pass_entity = render_pass_view.front();

        if (!registry.valid(render_pass_entity)) {
          SDL_Log("Can't bind GPU graphics pipeline. Render pass does not created.");
          return false;
        }

        auto* render_pass = registry.get<gpu_render_pass_component>(render_pass_entity).value;
        auto* render_pipeline = render_pipeline_view.get<opaque_render_pipeline_component>(render_pipeline_entity).value;

        SDL_BindGPUGraphicsPipeline(render_pass, render_pipeline);




      }

      return true;
    }
  };
} // namespace rendering::system