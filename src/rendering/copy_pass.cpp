#include "copy_pass.h"
#include "SDL3/SDL_log.h"
#include "component.h"

bool rendering::system::begin_copy_pass::update() noexcept {
  mark_to_destroy();

  using namespace component;

  const auto& copy_cmd_buffer_view = registry.view<gpu_command_buffer_component>();
  const auto& copy_cmd_buffer_entity = copy_cmd_buffer_view.front();

  if (!registry.valid(copy_cmd_buffer_entity)) {
    SDL_Log("Can't begin copy pass. Command buffer was not created.");
    return false;
  }

  auto* command_buffer = copy_cmd_buffer_view.get<gpu_command_buffer_component>(copy_cmd_buffer_entity).value;

  registry.emplace<gpu_copy_pass_component>(copy_cmd_buffer_entity, SDL_BeginGPUCopyPass(command_buffer));

  return true;
}

bool rendering::system::end_copy_pass::update() noexcept {
  using namespace component;

  const auto& view = registry.view<gpu_copy_pass_component>();

  if (view.size<gpu_copy_pass_component>()) mark_to_destroy();

  for (auto&& [entity, copy_pass] : view.each()) {
    SDL_EndGPUCopyPass(view.get<gpu_copy_pass_component>(entity).value);

    registry.remove<gpu_copy_pass_component>(entity);
  }

  return true;
}