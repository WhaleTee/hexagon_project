#pragma once
#include <cstdint>

enum component_tag : std::uint32_t {
  // command buffer tags

  copy = 0,
  render,

  // projection tag

  orthographic,
  perspective,
};