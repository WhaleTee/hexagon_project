#pragma once
#include "../../build/_deps/sdl-src/src/video/SDL_sysvideo.h"
#include "../ecs/system/base_system.h"
#include "component.h"
#include "entt/core/monostate.hpp"


// init systems:
// create device
// create window
// create copy command buffer
// create copy pass
//


// update systems:
// create render command buffer
// create render pass
//



struct render_primitive_system final : public ecs::system::system {
  explicit render_primitive_system(entt::registry& registry) : base_system(registry) {}

  ~render_primitive_system() noexcept override = default;

  bool update() noexcept override {
    // create buffer
    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(gpu_device);

    // create swapchain texture
    SDL_GPUTexture* swapchain_texture;
    // acquire it
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, &window_settings.width, &window_settings.height);

    if (swapchain_texture == nullptr) {
      SDL_SubmitGPUCommandBuffer(command_buffer);
      return true;
    }

    const glm::mat4& view = m_camera->view; // glm::lookAt(m_camera->position, m_camera->position + m_camera->front, m_camera->up);
    const glm::mat4& projection =
        m_camera->projection; // glm::perspective(glm::radians(m_camera->fov), (float)m_width / (float)m_height, m_camera->near, m_camera->far);

    SDL_GPUColorTargetInfo color_target_info{};
    color_target_info.texture = swapchain_texture;
    color_target_info.clear_color = {0.3f, 0.4f, 0.5f, 1.0f};
    color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
    color_target_info.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &color_target_info, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(render_pass, pipeline);

    Frustum frustum = Frustum::fromMatrix(projection * view);

    vertex_uniform vertex_uniform{};

    // find transform of a primitive

    SDL_PushGPUVertexUniformData(command_buffer, 0, &vertex_uniform, sizeof(vertex_uniform));

    // bind buffers and draw
    // get from somewhere vertex buffer
    SDL_GPUBufferCreateInfo buffer_create_info{};
    buffer_create_info.size = sizeof(vertex_uniform);
    buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    auto* vertex_buffer = SDL_CreateGPUBuffer(gpu_device, &buffer_create_info);
    SDL_GPUBufferBinding buffer_binding{vertex_buffer, 0};
    SDL_BindGPUVertexBuffers(render_pass, 0, &buffer_binding, 1);

    primitive_data primitive_data{};

    if (!primitive_data.indices.empty()) {
      SDL_GPUBufferBinding indices_binding{};
      SDL_BindGPUIndexBuffer(render_pass, &indices_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
      SDL_DrawGPUIndexedPrimitives(render_pass, primitive_data.indices.size(), 1, 0, 0, 0);
    } else SDL_DrawGPUPrimitives(render_pass, primitive_data.indices.size(), 1, 0, 0);

    return true;
  }
};