#pragma once
#include "../ecs/base_system.h"
#include "component.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_vertex_and_index_buffer final : ecs::system::base_system {
    explicit initialize_vertex_and_index_buffer(entt::registry& registry) : base_system(registry) {}

    ~initialize_vertex_and_index_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace data;

      const auto vertex_view =
          registry.view<const vertices_component, const indices_component>(entt::exclude<vertex_buffer_component, vertex_transfer_buffer_component>);

      if (!registry.valid(vertex_view.front())) { return true; }

      const auto gpu_device_view = registry.view<gpu_device_component>();
      const auto gpu_device_entity = gpu_device_view.front();

      if (!registry.valid(gpu_device_entity)) {
        SDL_Log("Cannot initialize vertex buffers. GPU device not found.");
        return false;
      }

      auto* gpu_device = gpu_device_view.get<gpu_device_component>(gpu_device_entity).value;
      const auto copy_pass_view = registry.view<gpu_copy_pass_component>();
      const auto copy_pass_entity = copy_pass_view.front();

      if (!registry.valid(copy_pass_entity)) {
        SDL_Log("Cannot initialize vertex buffers. Copy pass was not began.");
        return false;
      }

      auto* copy_pass = copy_pass_view.get<gpu_copy_pass_component>(copy_pass_view.front()).value;

      for (auto&& [entity, vertices, indices]: vertex_view.each()) {
        SDL_GPUBuffer* vertex_buffer = nullptr;
        SDL_GPUBuffer* index_buffer = nullptr;
        SDL_GPUTransferBuffer* vertex_transfer_buffer = nullptr;
        SDL_GPUTransferBuffer* index_transfer_buffer = nullptr;

        create_vertex_buffer(gpu_device, copy_pass, vertex_buffer, vertex_transfer_buffer, vertices.value);
        create_index_buffer(gpu_device, copy_pass, index_buffer, index_transfer_buffer, indices.value);

        registry.emplace<vertex_buffer_component>(entity, vertex_buffer);
        registry.emplace<index_buffer_component>(entity, index_buffer);
        registry.emplace<vertex_transfer_buffer_component>(entity, vertex_transfer_buffer);
        registry.emplace<index_transfer_buffer_component>(entity, index_transfer_buffer);
      }

      return true;
    }

    static void create_vertex_buffer(
        SDL_GPUDevice* gpu_device, SDL_GPUCopyPass* copy_pass, SDL_GPUBuffer* vertex_buffer, SDL_GPUTransferBuffer* vertex_transfer_buffer,
        const std::vector<data::vertex>& vertices) noexcept {
      using namespace data;

      SDL_GPUBufferCreateInfo buffer_info{};
      buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
      buffer_info.size = vertices.size() * sizeof(vertex);

      SDL_GPUTransferBufferCreateInfo transfer_info{};
      transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      transfer_info.size = buffer_info.size;

      vertex_buffer = SDL_CreateGPUBuffer(gpu_device, &buffer_info);
      vertex_transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_device, &transfer_info);
      auto* vertex_map = static_cast<vertex*>(SDL_MapGPUTransferBuffer(gpu_device, vertex_transfer_buffer, false));

      SDL_memcpy(vertex_map, vertices.data(), buffer_info.size);
      SDL_UnmapGPUTransferBuffer(gpu_device, vertex_transfer_buffer);

      SDL_GPUTransferBufferLocation buffer_location{vertex_transfer_buffer, 0};
      SDL_GPUBufferRegion buffer_region{vertex_buffer, 0, buffer_info.size};
      SDL_UploadToGPUBuffer(copy_pass, &buffer_location, &buffer_region, true);
    }

    static void create_index_buffer(
        SDL_GPUDevice* gpu_device, SDL_GPUCopyPass* copy_pass, SDL_GPUBuffer* index_buffer, SDL_GPUTransferBuffer* index_transfer_buffer,
        const std::vector<std::uint32_t>& indices) noexcept {
      SDL_GPUBufferCreateInfo indices_buffer_info{};
      indices_buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
      indices_buffer_info.size = indices.size() * sizeof(std::uint32_t);

      SDL_GPUTransferBufferCreateInfo indices_transfer_info{};
      indices_transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      indices_transfer_info.size = indices_buffer_info.size;

      index_buffer = SDL_CreateGPUBuffer(gpu_device, &indices_buffer_info);
      index_transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_device, &indices_transfer_info);
      auto* index_map = static_cast<std::uint32_t*>(SDL_MapGPUTransferBuffer(gpu_device, index_transfer_buffer, false));

      SDL_memcpy(index_map, indices.data(), indices_buffer_info.size);
      SDL_UnmapGPUTransferBuffer(gpu_device, index_transfer_buffer);

      SDL_GPUTransferBufferLocation indices_location{index_transfer_buffer, 0};
      SDL_GPUBufferRegion indices_region{index_buffer, 0, indices_buffer_info.size};
      SDL_UploadToGPUBuffer(copy_pass, &indices_location, &indices_region, true);
    }
  };
} // namespace rendering::system