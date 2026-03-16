#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_log.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_opaque_render_pipeline final : ecs::system::base_system {
    explicit initialize_opaque_render_pipeline(entt::registry& registry) : base_system(registry) {}

    ~initialize_opaque_render_pipeline() noexcept override = default;

    bool update() noexcept override {
      mark_to_destroy();

      using namespace component;
      const auto gpu_device_view = registry.view<gpu_device_component>();
      const auto gpu_device_entity = gpu_device_view.front();

      if (!registry.valid(gpu_device_entity)) {
        SDL_Log("Cannot create opaque pipeline. GPU device does not exist.");
        return false;
      }

      auto* gpu_device = gpu_device_view.get<gpu_device_component>(gpu_device_entity).value;

      SDL_GPUShader* vertexShader = Utils::loadShader("src/shaders/pbr.vert", 0, 1, SDL_GPU_SHADERSTAGE_VERTEX);
      SDL_GPUShader* fragmentShader = Utils::loadShader("src/shaders/pbr.frag", 10, 4, SDL_GPU_SHADERSTAGE_FRAGMENT);

      SDL_GPUVertexBufferDescription vertex_buffer_descriptions[1];
      vertex_buffer_descriptions[0].slot = 0;
      vertex_buffer_descriptions[0].pitch = sizeof(data::vertex);
      vertex_buffer_descriptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;

      SDL_GPUVertexAttribute vertex_attributes[2]{};
      vertex_attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0};
      vertex_attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, sizeof(float) * 3};

      SDL_GPUColorTargetDescription opaque_target_description{};
      opaque_target_description.format = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT;
      opaque_target_description.blend_state.enable_blend = false;

      SDL_GPUGraphicsPipelineCreateInfo pipeline_info{};
      pipeline_info.vertex_shader = vertexShader;
      pipeline_info.fragment_shader = fragmentShader;
      pipeline_info.vertex_input_state.num_vertex_buffers = 1;
      pipeline_info.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions;
      pipeline_info.vertex_input_state.num_vertex_attributes = 2;
      pipeline_info.vertex_input_state.vertex_attributes = vertex_attributes;
      pipeline_info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

      pipeline_info.target_info.has_depth_stencil_target = true;
      pipeline_info.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
      pipeline_info.target_info.num_color_targets = 1;
      pipeline_info.target_info.color_target_descriptions = &opaque_target_description;

      pipeline_info.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS;
      pipeline_info.depth_stencil_state.enable_depth_test = true;
      pipeline_info.depth_stencil_state.enable_depth_write = true;

      pipeline_info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;

      auto* pipeline = SDL_CreateGPUGraphicsPipeline(gpu_device_view.get<gpu_device_component>(gpu_device_entity).value, &pipeline_info);

      SDL_ReleaseGPUShader(gpu_device, vertexShader);
      SDL_ReleaseGPUShader(gpu_device, fragmentShader);

      if (pipeline == nullptr) {
        SDL_Log("Failed to create opaque pipeline: %s", SDL_GetError());
        return false;
      }

      registry.emplace<opaque_render_pipeline_component>(gpu_device_entity, pipeline);

      return true;
    }
  };
} // namespace rendering::system