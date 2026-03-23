#pragma once

enum component_tag : std::uint32_t {
  copy_cmd_buffer = 0,
  render_cmd_buffer,

  camera_entity,

  orthographic_projection,
  perspective_projection,
};