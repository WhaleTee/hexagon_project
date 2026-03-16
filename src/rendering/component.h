#pragma once
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"
#include "data.h"
#include "glm/vec4.hpp"

#include <vector>

namespace rendering::component {
  enum component_tag : std::uint32_t {
    // command buffer tags

    copy = 0,
    render,
  };

  struct window_destroy_request {};

  struct window_component {
    SDL_Window* value;
  };

  struct window_setting_component {
    SDL_WindowFlags window_flags;
    const char* name;
    Uint32 width;
    Uint32 height;
  };

  struct gpu_device_component {
    SDL_GPUDevice* value;
  };

  struct gpu_device_setting_component {
    SDL_GPUShaderFormat shader_format;
    char* driver_name;
    bool debug_mode;
  };

  struct opaque_render_pipeline_component {
    SDL_GPUGraphicsPipeline* value;
  };

  struct gpu_command_buffer_component {
    SDL_GPUCommandBuffer* value;
  };

  struct gpu_copy_pass_component {
    SDL_GPUCopyPass* value;
  };

  struct gpu_color_target_setting_component {
    SDL_GPUTexture* swapchainTexture;
    SDL_FColor clear_color;
    SDL_GPULoadOp load_op;
    SDL_GPUStoreOp store_op;
  };

  struct gpu_render_pass_component {
    SDL_GPURenderPass* value;
  };

  struct vertices_component {
    std::vector<data::vertex> value;
  };

  struct vertex_buffer_component {
    SDL_GPUBuffer* value;
  };

  struct vertex_transfer_buffer_component {
    SDL_GPUTransferBuffer* value;
  };

  struct indices_component {
    std::vector<std::uint32_t> value;
  };

  struct index_buffer_component {
    SDL_GPUBuffer* value;
  };

  struct index_transfer_buffer_component {
    SDL_GPUTransferBuffer** value;
  };
} // namespace rendering::component