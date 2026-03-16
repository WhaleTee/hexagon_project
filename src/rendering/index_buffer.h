#pragma once
#include "component.h"

#include "../ecs/system/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  struct initialize_index_buffer final : ecs::system::system {
    explicit initialize_index_buffer(entt::registry& registry) : base_system(registry) {}

    ~initialize_index_buffer() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace data;

      const auto device_view = registry.view<gpu_device_component>();
      auto* device = device_view.get<gpu_device_component>(device_view.front()).value;

      const auto copy_pass_view = registry.view<gpu_copy_pass_component>();
      auto* copy_pass = copy_pass_view.get<gpu_copy_pass_component>(copy_pass_view.front()).value;

      const auto view = registry.view<const indices_component>(entt::exclude<vertex_buffer_component, vertex_transfer_buffer_component>);

      for (auto&& [entity, indices]: view.each()) {
        SDL_GPUBufferCreateInfo indices_buffer_info{};
        indices_buffer_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        indices_buffer_info.size = indices.value.size() * sizeof(uint32_t);
        auto* indices_buffer = SDL_CreateGPUBuffer(device, &indices_buffer_info);

        SDL_GPUTransferBufferCreateInfo indices_transfer_info{};
        indices_transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        indices_transfer_info.size = indices_buffer_info.size;
        auto* indices_transfer_buffer = SDL_CreateGPUTransferBuffer(device, &indices_transfer_info);
        auto* index_map = static_cast<uint32_t*>(SDL_MapGPUTransferBuffer(device, indices_transfer_buffer, false));
        SDL_memcpy(index_map, indices.value.data(), indices_buffer_info.size);
        SDL_UnmapGPUTransferBuffer(device, indices_transfer_buffer);

        SDL_GPUTransferBufferLocation indices_location{indices_transfer_buffer, 0};
        SDL_GPUBufferRegion indices_region{indices_buffer, 0, indices_buffer_info.size};
        SDL_UploadToGPUBuffer(copy_pass, &indices_location, &indices_region, true);
      }

      return true;
    }
  };
}; // namespace rendering::system