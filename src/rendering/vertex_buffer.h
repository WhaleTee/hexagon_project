#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_vertex_buffer final : ecs::system::system {
    explicit initialize_vertex_buffer(entt::registry& registry) : base_system(registry) {}

    ~initialize_vertex_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace data;

      const auto device_view = registry.view<gpu_device_component>();
      auto* device = device_view.get<gpu_device_component>(device_view.front()).value;

      const auto copy_pass_view = registry.view<gpu_copy_pass_component>();
      auto* copy_pass = copy_pass_view.get<gpu_copy_pass_component>(copy_pass_view.front()).value;

      const auto view = registry.view<const vertices_component>(entt::exclude<vertex_buffer_component, vertex_transfer_buffer_component>);

      for (auto&& [entity, vertices]: view.each()) {
        SDL_GPUBufferCreateInfo buffer_info{};
        buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        buffer_info.size = vertices.value.size() * sizeof(vertex);
        auto* buffer = SDL_CreateGPUBuffer(device, &buffer_info);

        SDL_GPUTransferBufferCreateInfo transfer_info{};
        transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transfer_info.size = buffer_info.size;
        auto* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_info);

        auto* data = static_cast<vertex*>(SDL_MapGPUTransferBuffer(device, transfer_buffer, false));
        SDL_memcpy(data, vertices.value.data(), buffer_info.size);
        SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

        SDL_GPUTransferBufferLocation buffer_location{transfer_buffer, 0};
        SDL_GPUBufferRegion buffer_region{buffer, 0, buffer_info.size};
        SDL_UploadToGPUBuffer(copy_pass, &buffer_location, &buffer_region, true);

        registry.emplace<vertex_buffer_component>(entity, buffer);
        registry.emplace<vertex_transfer_buffer_component>(entity, transfer_buffer);
      }

      return true;
    }
  };
} // namespace rendering::system