#pragma once
#include "hexagon.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <unordered_set>

namespace hex {
  class hexmap {
    glm::vec3 position{};
    int map_size;
    float hex_size;
    float hex_height;
    bool is_flat_top;
    std::unordered_set<hexagon> hexes;

  public:
    hexmap(const std::size_t& size, const float& hex_size, const float& hex_height, const bool& flat_top);

    void build_hexmap() noexcept;

    [[nodiscard]] std::unordered_set<hexagon> get_hexes() const noexcept;

    [[nodiscard]] hexagon get_hexagon(const glm::vec2& world_position) const noexcept;

    [[nodiscard]] glm::vec3 get_hexagon_world_position(const hexagon& hex) const noexcept;

    void set_position(const glm::vec3& value) noexcept;
  };
} // namespace hexmap
