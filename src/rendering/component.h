#pragma once
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"
#include "data.h"
#include <vector>
#include "entt/entity/entity.hpp"

namespace rendering::component {
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

  struct render_pipeline_component {
    SDL_GPUGraphicsPipeline* value;
  };

  struct render_pipeline_release_request {};

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

  struct vertex_and_index_buffer_release_request {};

  struct vertex_buffer_component {
    SDL_GPUBuffer* value;
  };

  struct vertex_transfer_buffer_component {
    SDL_GPUTransferBuffer* value;
  };

  struct indices_component {
    std::vector<Uint32> value;
  };

  struct index_buffer_component {
    SDL_GPUBuffer* value;
  };

  struct index_transfer_buffer_component {
    SDL_GPUTransferBuffer* value;
  };

  struct render_model_component {
    std::vector<entt::entity> children;
  };
} // namespace rendering::component