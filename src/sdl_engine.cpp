#include "sdl_engine.h"
#include "ecs/game_world.h"
#include "event/game_quit_event.h"
#include "event/game_world_destroy_event.h"
#include "rendering/command_buffer.h"
#include "rendering/component.h"
#include "rendering/copy_pass.h"
#include "rendering/opaque_render_pipeline.h"
#include "rendering/render_command_buffer.h"
#include "rendering/render_pass.h"
#include "rendering/vertex_buffer.h"
#include "rendering/window.h"
#include <numeric>

namespace {
  using namespace rendering::component;
  using namespace rendering::system;

  struct hexagon_game_world final : ecs::game_world {
    hexagon_game_world() {

      const auto window_entity = registry.create();

      auto& gpu_device_settings = registry.emplace<gpu_device_setting_component>(window_entity);
      gpu_device_settings.shader_format = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;

      auto& window_settings = registry.emplace<window_setting_component>(window_entity);
      window_settings.name = "Hexagon Field Demo";
      window_settings.height = 1366;
      window_settings.width = 768;

      system_manager.create_system<create_window>();
      system_manager.create_system<initialize_opaque_render_pipeline>();

      system_manager.create_system<initialize_command_buffer<component_tag::copy>>();
      system_manager.create_system<begin_copy_pass>();
      system_manager.create_system<initialize_vertex_and_index_buffer>();
      system_manager.create_system<end_copy_pass>();
      system_manager.create_system<submit_copy_command_buffer<component_tag::copy>>();


      system_manager.create_system<initialize_command_buffer<component_tag::render>>();
      system_manager.create_system<begin_render_pass>();

      system_manager.create_system<end_render_pass>();
      system_manager.create_system<submit_copy_command_buffer<component_tag::render>>();
    }

    ~hexagon_game_world() noexcept override = default;

    void dispatch_event(SDL_Event* event) noexcept override {
      switch (event->type) {
      // case SDL_EVENT_QUIT:
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:

        dispatcher.trigger<event::game_quit_event>();
        // for (auto [entity, gpu, window]: registry.view<ecs::component::gpu_device_component, ecs::component::window_component>().each()) {
        //   registry.emplace<ecs::component::graphics_destroy_request_tag>(entity);
        // }
        break;

      default:
        break;
      }
    }
  };

  hexagon_game_world* world;
  bool destroy_game_world{false};

  void on_game_world_destroy() noexcept {
    destroy_game_world = true;
  }
} // namespace

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