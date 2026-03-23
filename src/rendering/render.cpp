#include "render.h"
#include "../camera/component.h"
#include "../component_tag.h"
#include "../transform/component.h"
#include "SDL3/SDL_gpu.h"
#include "SDL3/SDL_log.h"
#include "component.h"

#include <iostream>

bool rendering::system::render_mvp_system::update() noexcept {
  using namespace component;
  using namespace camera::component;
  using namespace transform::component;

  const auto& render_pipeline_view = registry.view<render_pipeline_component>();
  const auto& render_pipeline_entity = render_pipeline_view.front();

  if (registry.valid(render_pipeline_entity)) {
    const auto& render_cmd_buffer_view = registry.view<gpu_command_buffer_component, entt::tag<component_tag::render_cmd_buffer>>();
    const auto& render_cmd_buffer_entity = render_cmd_buffer_view.front();

    if (!registry.valid(render_cmd_buffer_entity)) {
      SDL_Log("Can't bind GPU graphics pipeline. Render pass does not created.");
      return false;
    }

    const auto& render_pass_view = registry.view<gpu_render_pass_component>();
    const auto& render_pass_entity = render_pass_view.front();

    if (!registry.valid(render_pass_entity)) {
      SDL_Log("Can't bind GPU graphics pipeline. Render pass does not created.");
      return false;
    }

    auto* render_cmd_buffer = registry.get<gpu_command_buffer_component>(render_cmd_buffer_entity).value;
    auto* render_pass = registry.get<gpu_render_pass_component>(render_pass_entity).value;
    auto* render_pipeline = render_pipeline_view.get<render_pipeline_component>(render_pipeline_entity).value;

    SDL_BindGPUGraphicsPipeline(render_pass, render_pipeline);

    const auto& camera_view = registry.view<model_matrix_component, view_matrix_component, projection_component, entt::tag<component_tag::camera_entity>>();
    const auto& camera_entity = camera_view.front();

    if (!registry.valid(camera_entity)) return true;

    const auto& camera_view_matrix = registry.get<view_matrix_component>(camera_entity).value;
    const auto& camera_projection_matrix = registry.get<projection_component>(camera_entity).value;

    for (auto&& [entity, render_model]: registry.view<render_model_component>().each()) {
      if (registry.all_of<model_matrix_component>(entity)) {
        const auto& model_matrix = registry.get<model_matrix_component>(entity).value;
        const auto mvp = camera_projection_matrix * camera_view_matrix * model_matrix;

        SDL_PushGPUVertexUniformData(render_cmd_buffer, 0, &mvp, sizeof(mvp));

        for (auto&& child: render_model.children) {
          if (registry.all_of<vertices_component, vertex_buffer_component, indices_component, index_buffer_component>(child)) {
            const auto [vertices, vertex_buffer, indices, index_buffer] =
                registry.get<vertices_component, vertex_buffer_component, indices_component, index_buffer_component>(child);
            // std::cout << "--------- vertices of " << static_cast<std::uint32_t>(child) << " ----------" << std::endl;
            // for (auto vertex : vertices.value) {
            //   std::cout << '[' << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ']' << std::endl;
            // }

            SDL_GPUBufferBinding vertex_binding{vertex_buffer.value, 0};
            SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_binding, 1);

            SDL_GPUBufferBinding index_binding{index_buffer.value, 0};
            SDL_BindGPUIndexBuffer(render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

            SDL_DrawGPUIndexedPrimitives(render_pass, indices.value.size(), 1, 0, 0, 0);
          }
        }
      }
    }
  }

  return true;
}