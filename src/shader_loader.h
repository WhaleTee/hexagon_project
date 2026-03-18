#pragma once
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_log.h"
#include <format>
#include <string>

inline SDL_GPUShader* load_shader(
    SDL_GPUDevice* device, const std::string& shader_filename, const Uint32 sampler_count, const Uint32 uniform_buffer_count, const Uint32 storage_buffer_count,
    const Uint32 storage_texture_count) {
  SDL_GPUShaderStage stage;
  if (SDL_strstr(shader_filename.c_str(), ".vert")) {
    stage = SDL_GPU_SHADERSTAGE_VERTEX;
  } else if (SDL_strstr(shader_filename.c_str(), ".frag")) {
    stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
  } else {
    SDL_Log("Invalid shader stage!");
    return nullptr;
  }

  const auto full_path = std::format(R"({}assets\shaders\compiled\{}.spv)", SDL_GetBasePath(), shader_filename);
  const std::string entry_point = "main";
  size_t code_size;
  auto* code = static_cast<Uint8*>(SDL_LoadFile(full_path.c_str(), &code_size));

  if (!code) {
    SDL_Log("Failed to load shader from disk! %s", full_path.c_str());
    return nullptr;
  }

  SDL_GPUShaderCreateInfo shader_info{};
  shader_info.code = code;
  shader_info.code_size = code_size;
  shader_info.entrypoint = entry_point.c_str();
  shader_info.format = SDL_GPU_SHADERFORMAT_SPIRV;
  shader_info.stage = stage;
  shader_info.num_samplers = sampler_count;
  shader_info.num_uniform_buffers = uniform_buffer_count;
  shader_info.num_storage_buffers = storage_buffer_count;
  shader_info.num_storage_textures = storage_texture_count;

  SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shader_info);

  SDL_free(code);

  if (!shader) {
    SDL_Log("Failed to create shader! %s", SDL_GetError());
    return nullptr;
  }

  return shader;
}