#include "sdl_engine.h"

#include "ecs/systems/graphics_destroy_system.h"
#include "ecs/systems/graphics_initialization_system.h"
#include "ecs/world.h"


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
  struct game_world final : ecs::world {
    game_world() {
      // register scene manager
      // create scene, register it
      // in the scene create needed entities


      auto& graphic_system = system_manager.create_system<ecs::graphics_initialization_system>(dispatcher);
      auto& graphic_destroy_system = system_manager.create_system<ecs::graphics_destroy_system>(dispatcher);
      auto entity = registry.create();
      auto& gpu_device_setting_component = registry.emplace<ecs::component::gpu_device_setting_component>(entity);
      auto& window_setting_component = registry.emplace<ecs::component::window_setting_component>(entity);
      gpu_device_setting_component.shader_format = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
      window_setting_component.name = "window";
      window_setting_component.height = 800;
      window_setting_component.width = 800;
    }

    ~game_world() noexcept override = default;

    void dispatch_event(SDL_Event* event) noexcept override {
      switch (event->type) {
      case SDL_EVENT_QUIT:
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:

        dispatcher.trigger<event::game_quit_event>();
        break;

      default:
        break;
      }
    }
  };

  game_world* world;
}

SDL_AppResult SDL_AppInit(void** appState, int argc, char** argv) {
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO) || !SDL_InitSubSystem(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;
  world = new game_world();
  world->run();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appState) {
  if (!world) return SDL_APP_SUCCESS;
  world->update();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appState, SDL_Event* event) {
  if (world) world->dispatch_event(event);
  if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appState, SDL_AppResult result) {
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
  SDL_Quit();
}