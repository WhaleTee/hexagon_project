#pragma once
#include "../ecs/base_system.h"
#include "SDL3/SDL_gpu.h"
#include "data.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class initialize_vertex_and_index_buffer final : public ecs::system::base_system {
    static void create_vertex_buffer(
        SDL_GPUDevice* gpu_device, SDL_GPUCopyPass* copy_pass, SDL_GPUBuffer*& vertex_buffer, SDL_GPUTransferBuffer*& transfer_buffer,
        const std::vector<data::vertex>& vertices) noexcept {
      using namespace data;

      SDL_GPUBufferCreateInfo buffer_info{};
      buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
      buffer_info.size = vertices.size() * sizeof(vertex);

      SDL_GPUTransferBufferCreateInfo transfer_info{};
      transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      transfer_info.size = buffer_info.size;

      vertex_buffer = SDL_CreateGPUBuffer(gpu_device, &buffer_info);
      transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);
      auto* vertex_map = static_cast<vertex*>(SDL_MapGPUTransferBuffer(gpu_device, transfer_buffer, false));

      SDL_memcpy(vertex_map, vertices.data(), buffer_info.size);
      SDL_UnmapGPUTransferBuffer(gpu_device, transfer_buffer);

      SDL_GPUTransferBufferLocation buffer_location{transfer_buffer, 0};
      SDL_GPUBufferRegion buffer_region{vertex_buffer, 0, buffer_info.size};
      SDL_UploadToGPUBuffer(copy_pass, &buffer_location, &buffer_region, true);
    }

    static void create_index_buffer(
        SDL_GPUDevice* gpu_device, SDL_GPUCopyPass* copy_pass, SDL_GPUBuffer*& index_buffer, SDL_GPUTransferBuffer*& transfer_buffer,
        const std::vector<std::uint32_t>& indices) noexcept {
      SDL_GPUBufferCreateInfo buffer_info{};
      buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
      buffer_info.size = indices.size() * sizeof(std::uint32_t);

      SDL_GPUTransferBufferCreateInfo transfer_info{};
      transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      transfer_info.size = buffer_info.size;

      index_buffer = SDL_CreateGPUBuffer(gpu_device, &buffer_info);
      transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);
      auto* index_map = static_cast<std::uint32_t*>(SDL_MapGPUTransferBuffer(gpu_device, transfer_buffer, false));

      SDL_memcpy(index_map, indices.data(), buffer_info.size);
      SDL_UnmapGPUTransferBuffer(gpu_device, transfer_buffer);

      SDL_GPUTransferBufferLocation indices_location{transfer_buffer, 0};
      SDL_GPUBufferRegion indices_region{index_buffer, 0, buffer_info.size};
      SDL_UploadToGPUBuffer(copy_pass, &indices_location, &indices_region, true);
    }

  public:
    explicit initialize_vertex_and_index_buffer(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

  class release_vertex_and_index_buffer final : public ecs::system::base_system {
  public:
    explicit release_vertex_and_index_buffer(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace rendering::system