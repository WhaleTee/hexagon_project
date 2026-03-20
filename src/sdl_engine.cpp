#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include "camera/camera.h"
#include "camera/normalize_camera_up.h"
#include "ecs/game_world.h"
#include "event/game_quit_event.h"
#include "event/game_world_destroy_event.h"
#include "hexagon/hexmap.h"
#include "hexagon/hexmath.h"
#include "rendering/command_buffer.h"
#include "rendering/component.h"
#include "rendering/copy_pass.h"
#include "rendering/render_pass.h"
#include "rendering/render_pipeline.h"
#include "rendering/vertex_buffer.h"
#include "rendering/window.h"
#include "world_space/rotation.h"
#include "world_space/view.h"
#include <iostream>
#include <numeric>

namespace {
  using namespace rendering::component;
  using namespace rendering::system;
  using namespace space::system;
  using namespace space::component;
  using namespace camera::system;
  using namespace camera::component;

  struct hexagon_game_world final : ecs::game_world {
    hexagon_game_world() {

      // create entities
      const auto window_entity = registry.create();
      const auto camera_entity = registry.create();

      hex::hexmap hexmap{3, 100.f, 1, true};
      constexpr glm::vec3 position{1280.f / 2, 720.f / 2, 0};
      hexmap.set_position(position);
      for (const auto hex: hexmap.get_hexes()) {
        const auto hexagon_entity = registry.create();
        std::vector<rendering::data::vertex> vertices{6};
        auto hex_vertices = hex::hexmath::get_hexagon_vertices(hex);
        for (int i = 0; i < 6; i++) {
          vertices[i] = rendering::data::vertex{hex_vertices[i], glm::vec4{1.f, 1.f, 1.f, 1.f}};
        }
        registry.emplace<vertices_component>(hexagon_entity, std::move(vertices));
        registry.emplace<indices_component>(hexagon_entity, std::move(hex::hexagon::get_draw_line_strip_indices()));
        registry.emplace<position_component>(hexagon_entity, hexmap.get_hexagon_world_position(hex));
      }

      // config gpu device
      auto& gpu_device_settings = registry.emplace<gpu_device_setting_component>(window_entity);
      gpu_device_settings.shader_format = SDL_GPU_SHADERFORMAT_SPIRV;
      gpu_device_settings.debug_mode = true;

      auto& color_target_settings = registry.emplace<gpu_color_target_setting_component>(window_entity);
      color_target_settings.clear_color = {.1f, .1f, .1f, 1.f};
      color_target_settings.load_op = SDL_GPU_LOADOP_CLEAR;
      color_target_settings.store_op = SDL_GPU_STOREOP_STORE;

      // config window
      auto& window_settings = registry.emplace<window_setting_component>(window_entity);
      window_settings.name = "Hexagon Field Demo";
      window_settings.height = 720;
      window_settings.width = 1280;

      // config camera
      auto& camera_setting = registry.emplace<camera_setting_component>(camera_entity);
      camera_setting.width = window_settings.width;
      camera_setting.height = window_settings.height;
      camera_setting.near = 0.1f;
      camera_setting.far = 100.f;

      // auto camera_position = glm::vec3{0.f, 0.f, -1.f};
      // glm::quat pitch = glm::angleAxis(glm::radians(45.f), glm::vec3(1, 0, 0));
      // glm::quat yaw = glm::angleAxis(glm::radians(45.f), glm::vec3(0, 1, 0));
      // registry.emplace<position_component>(camera_entity, camera_position);
      // auto mat = glm::mat4_cast(pitch * yaw);


      // std::cout << "-------- matrix start --------" << std::endl;
      // std::cout << '[' << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2]<< ", " << mat[0][3] << ']' << std::endl;
      // std::cout << '[' << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2]<< ", " << mat[1][3] << ']' << std::endl;
      // std::cout << '[' << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2]<< ", " << mat[2][3] << ']' << std::endl;
      // std::cout << '[' << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2]<< ", " << mat[3][3] << ']' << std::endl;
      // std::cout << "-------- matrix end --------" << std::endl;

      registry.emplace<orientation_component>(camera_entity, glm::mat4{1});
      registry.emplace<view_matrix_component>(camera_entity, glm::mat4{1});
      registry.emplace<projection_component>(camera_entity);
      registry.emplace<entt::tag<component_tag::orthographic>>(camera_entity);

      // auto& camera_rotation = registry.emplace<rotation_request>(camera_entity);
      // camera_rotation.x = 45;
      // camera_rotation.y = 25;
      // camera_rotation.z = -35;

      // init systems
      system_manager.create_system<create_window>();
      system_manager.create_system<initialize_render_pipeline>();
      system_manager.create_system<release_render_pipeline>();

      system_manager.create_system<initialize_command_buffer<component_tag::copy>>();
      system_manager.create_system<begin_copy_pass>();
      system_manager.create_system<initialize_vertex_and_index_buffer>();
      system_manager.create_system<release_vertex_and_index_buffer>();
      system_manager.create_system<end_copy_pass>();
      system_manager.create_system<submit_command_buffer<component_tag::copy>>();

      // camera systems
      system_manager.create_system<rotation_system>();
      system_manager.create_system<view_matrix_system>();
      // system_manager.create_system<normalize_camera_up_system>();
      system_manager.create_system<camera_orthographic_projection_system>();

      // rendering systems
      system_manager.create_system<initialize_command_buffer<component_tag::render>>();
      system_manager.create_system<begin_render_pass>();
      system_manager.create_system<render_mvp_system>();
      system_manager.create_system<end_render_pass>();
      system_manager.create_system<submit_command_buffer<component_tag::render>>();

      system_manager.create_system<destroy_window>(dispatcher);
    }

    ~hexagon_game_world() noexcept override = default;

    void dispatch_event(SDL_Event* event) noexcept override {
      switch (event->type) {
      // case SDL_EVENT_QUIT:
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED:

        event::game_quit_event::invoker(this->dispatcher);
        for (auto&& entity: registry.view<window_component>()) {
          registry.emplace<window_destroy_request>(entity);
        }

        for (auto&& entity: registry.view<render_pipeline_component>()) {
          registry.emplace<render_pipeline_release_request>(entity);
        }

        for (auto&& entity: registry.view<vertex_buffer_component, vertex_transfer_buffer_component>()) {
          registry.emplace<vertex_and_index_buffer_release_request>(entity);
        }
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
  if (!world) return SDL_APP_SUCCESS;
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