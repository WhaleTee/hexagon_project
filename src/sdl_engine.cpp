#include "sdl_engine.h"

#include "ecs/game_world.h"
#include "ecs/system/graphics_destroy_system.h"
#include "ecs/system/graphics_initialization_system.h"
#include "event/game_quit_event.h"
#include "event/game_world_destroy_event.h"

#include <numeric>
#include <vector>

// initialization steps: init sdl systems, create world
// in the world: create scenes, register systems and create entities
// run steps: run world, scenes, systems
// what is a scene? an entity manager, I suppose.
// It is creating them in construction stage, and manages their components while app is running.
// Also, it will respond for signals and other stuff and achieve managing of entities. Why?
// Can we just use some singleton state tag? And managing all things by the systems?
// Read configs in systems, in parallel maybe, or non-blocking way at least.
// Create


namespace {
  struct hexagon_game_world final : ecs::game_world {
    hexagon_game_world() {
      // register scene manager
      // create scene, register it
      // in the scene create needed entities


      auto& graphic_system = system_manager.create_system<ecs::system::graphics_initialization_system>(dispatcher);
      auto& graphic_destroy_system = system_manager.create_system<ecs::system::graphics_destroy_system>(dispatcher);
      auto entity = registry.create();
      auto& gpu_device_setting_component = registry.emplace<ecs::component::gpu_device_setting_component>(entity);
      auto& window_setting_component = registry.emplace<ecs::component::window_setting_component>(entity);
      gpu_device_setting_component.shader_format = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
      window_setting_component.name = "window";
      window_setting_component.height = 800;
      window_setting_component.width = 800;
    }

    ~hexagon_game_world() noexcept override = default;

    void dispatch_event(SDL_Event* event) noexcept override {
      switch (event->type) {
      // case SDL_EVENT_QUIT:
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:

        dispatcher.trigger<event::game_quit_event>();
        for (auto [entity, gpu, window]: registry.view<ecs::component::gpu_device_component, ecs::component::window_component>().each()) {
          registry.emplace<ecs::component::graphics_destroy_request_tag>(entity);
        }
        break;

      default:
        break;
      }
    }
  };

  hexagon_game_world* world;
  bool destroy_game_world{false};
} // namespace

void on_game_world_destroy() noexcept {
  destroy_game_world = true;
}

struct gpu_vertex_buffer_component {
  SDL_GPUBuffer* buffer;
  uint32_t size;
};

struct gpu_transfer_buffer_component {
  SDL_GPUTransferBuffer* buffer;
  std::vector<uint32_t> offsets;
  std::vector<uint32_t> sizes;
  explicit gpu_transfer_buffer_component(SDL_GPUTransferBuffer* buffer, const std::vector<uint32_t>& offsets, const std::vector<uint32_t>& sizes) :
  buffer(buffer),
  offsets(offsets),
  sizes(sizes) {}
};

struct gpu_transfer_upload_buffer_component {
  uint32_t size{};
  std::vector<entt::any> data{};
};

struct gpu_transfer_download_buffer_component {
  uint32_t size{};
  std::vector<entt::any> data{};
};

class create_gpu_transfer_buffers final : public ecs::system::system {
public:
  explicit create_gpu_transfer_buffers(entt::registry& registry) noexcept : base_system(registry) {}
  ~create_gpu_transfer_buffers() noexcept override = default;
  bool update() noexcept override {
    using namespace ecs::component;

    auto entities = registry.view<gpu_transfer_upload_buffer_component>();
    const auto entity_count = entities.size();

    std::vector<uint32_t> buffer_sizes(entity_count);
    std::vector<uint32_t> buffer_offsets(entity_count);

    auto buffer_sizes_it = buffer_sizes.begin();
    auto buffer_offsets_it = buffer_offsets.begin();

    SDL_GPUTransferBufferCreateInfo transfer_buffer_info{SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD};

    for (auto [entity, upload_buffer]: entities.each()) {
      (*buffer_sizes_it) = upload_buffer.size;

      if (buffer_offsets_it == buffer_offsets.begin()) (*buffer_offsets_it) = 0;
      else (*buffer_offsets_it) = *(buffer_offsets_it - 1) + upload_buffer.size;

      ++buffer_sizes_it;
      ++buffer_offsets_it;
    }

    transfer_buffer_info.size = std::accumulate(buffer_sizes.begin(), buffer_sizes.end(), 0ull, [](const auto sum, const auto size) {
      return sum + size;
    });

    for (auto [entity, device, buffer]: registry.view<gpu_device_component, gpu_vertex_buffer_component>().each()) {
      registry.emplace_or_replace<gpu_transfer_buffer_component>(entity, SDL_CreateGPUTransferBuffer(device.value_ptr, &transfer_buffer_info), buffer_offsets, buffer_sizes);
    }

    SDL_GPUBuffer* buffer{};

    return true;
  }
};


class vertex_buffer_create_system final : public ecs::system::system {
public:
  explicit vertex_buffer_create_system(entt::registry& registry) noexcept : base_system(registry) {}
  ~vertex_buffer_create_system() noexcept override = default;
  bool update() noexcept override {
    auto* device = registry.ctx().get<SDL_GPUDevice*>();
    auto buffer = registry.create();
    auto& buffer_component = registry.emplace<gpu_vertex_buffer_component>(buffer);
    SDL_GPUBufferCreateInfo info{SDL_GPU_BUFFERUSAGE_VERTEX, 0};
    SDL_GPUBuffer* vertex_buffer = SDL_CreateGPUBuffer(device, &info);
    SDL_GPUTransferBufferLocation location{};
    return true;
  }
};

SDL_AppResult SDL_AppInit(void** appState, int argc, char** argv) {
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO) || !SDL_InitSubSystem(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;
  world = new hexagon_game_world();

  world->register_event_listener<event::game_world_destroy_event, &on_game_world_destroy>();

  world->run();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appState) {
  if (!world) { return SDL_APP_SUCCESS; }
  world->update();
  if (destroy_game_world) {
    delete world;
    world = nullptr;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appState, SDL_Event* event) {
  if (world) world->dispatch_event(event);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appState, SDL_AppResult result) {
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
  SDL_Quit();
}