#pragma once
#include "../ecs/base_system.h"
#include "../shader_loader.h"
#include "SDL3/SDL_log.h"
#include "component.h"

#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_render_pipeline final : ecs::system::base_system {
    explicit initialize_render_pipeline(entt::registry& registry) : base_system(registry) {}

    ~initialize_render_pipeline() noexcept override = default;

    bool update() noexcept override {
      mark_to_destroy();

      using namespace component;
      const auto& gpu_device_view = registry.view<gpu_device_component, window_component>();
      const auto& gpu_device_entity = gpu_device_view.front();

      if (!registry.valid(gpu_device_entity)) {
        SDL_Log("Cannot create pipeline. GPU device does not exist.");
        return false;
      }

      auto* gpu_device = gpu_device_view.get<gpu_device_component>(gpu_device_entity).value;
      auto* window = gpu_device_view.get<window_component>(gpu_device_entity).value;

      SDL_GPUShader* vertexShader = load_shader(gpu_device, "mvp.vert", 0, 1, 0, 0);
      SDL_GPUShader* fragmentShader = load_shader(gpu_device, "mvp.frag", 0, 0, 0, 0);

      SDL_GPUVertexBufferDescription vertex_buffer_descriptions[1];
      vertex_buffer_descriptions[0].instance_step_rate = 0;
      vertex_buffer_descriptions[0].slot = 0;
      vertex_buffer_descriptions[0].pitch = sizeof(data::vertex);
      vertex_buffer_descriptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;

      SDL_GPUVertexAttribute vertex_attributes[2]{};
      vertex_attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0};
      vertex_attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, sizeof(float) * 3};

      SDL_GPUColorTargetDescription color_target = {};
      color_target.format = SDL_GetGPUSwapchainTextureFormat(gpu_device, window);

      SDL_GPUGraphicsPipelineCreateInfo pipeline_info{};
      pipeline_info.vertex_shader = vertexShader;
      pipeline_info.fragment_shader = fragmentShader;
      pipeline_info.primitive_type = SDL_GPU_PRIMITIVETYPE_LINESTRIP;
      pipeline_info.vertex_input_state.num_vertex_buffers = 1;
      pipeline_info.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions;
      pipeline_info.vertex_input_state.num_vertex_attributes = 2;
      pipeline_info.vertex_input_state.vertex_attributes = vertex_attributes;
      pipeline_info.target_info.num_color_targets = 1;
      pipeline_info.target_info.color_target_descriptions = &color_target;

      auto* pipeline = SDL_CreateGPUGraphicsPipeline(gpu_device, &pipeline_info);

      SDL_ReleaseGPUShader(gpu_device, vertexShader);
      SDL_ReleaseGPUShader(gpu_device, fragmentShader);

      if (pipeline == nullptr) {
        SDL_Log("Failed to create opaque pipeline: %s", SDL_GetError());
        return false;
      }

      registry.emplace<render_pipeline_component>(gpu_device_entity, pipeline);

      return true;
    }
  };

  struct release_render_pipeline final : ecs::system::base_system {
    explicit release_render_pipeline(entt::registry& registry) : base_system(registry) {}

    ~release_render_pipeline() noexcept override = default;

    bool update() noexcept override {
      using namespace component;

      const auto gpu_device_view = registry.view<gpu_device_component>();
      const auto gpu_device_entity = gpu_device_view.front();

      if (!registry.valid(gpu_device_entity)) {
        SDL_Log("Cannot destroy pipeline. GPU device has been destroyed.");
        return false;
      }

      auto* gpu_device = gpu_device_view.get<gpu_device_component>(gpu_device_entity).value;

      for (auto [entity, pipeline]: registry.view<render_pipeline_component, render_pipeline_release_request>().each()) {
        mark_to_destroy();

        SDL_ReleaseGPUGraphicsPipeline(gpu_device, pipeline.value);

        registry.remove<render_pipeline_component, render_pipeline_release_request>(entity);
      }

      return true;
    }
  };
} // namespace rendering::system