#pragma once
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_video.h"

namespace ecs::component {
  struct window_component {
    SDL_Window* value_ptr;
  };

  struct gpu_device_component {
    SDL_GPUDevice* value_ptr;
  };

  struct gpu_device_setting_component {
    SDL_GPUShaderFormat shader_format;
    char* driver_name;
    bool debug_mode;
  };

  struct window_setting_component {
    SDL_WindowFlags window_flags;
    const char* name;
    int width;
    int height;
  };

  struct graphics_destroy_request_tag {};
}