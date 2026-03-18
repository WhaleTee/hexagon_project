#pragma once
#include "hexagon.h"
#include <glm/vec3.hpp>
#include <set>

namespace hexmap {
  class hexmap {
    glm::vec3 position{};
    std::size_t map_size;
    float hex_size;
    float hex_height;
    bool is_flat_top;
    std::set<hexagon> hexes{};

  public:
    hexmap(const std::size_t size, const float hex_size, const float hex_height, const bool flat_top) :
        map_size(size), hex_size(hex_size), hex_height(hex_height), is_flat_top(flat_top) {
      for (int i = -3; i < 4; i++) {
        for (int j = -3; j < 4; j++) {
          hexes.emplace(true, i, j, 50, 1);
        }
      }
    }

    [[nodiscard]] glm::vec3 get_hexagon_world_position(const hexagon& hex) const {
      return hex.get_world_position() + position;
    }

    [[nodiscard]] std::vector<glm::vec3> get_hexagon_vertices_world_position(const hexagon& hex) const {
      std::vector<glm::vec3> vertices = hex.get_vertices();
      for (auto& vertex : vertices) {
        vertex += get_hexagon_world_position(hex);
      }
      return vertices;
    }

    [[nodiscard]] hexagon_coordinates get_hexagon_in_position(const glm::vec2& world_position) const {
      const float sqrt_3_b_3 = sqrtf(3) / 3;
      const auto x = world_position.x / hex_size;
      const auto y = world_position.y / hex_size;
      const auto q = is_flat_top ? 2.f / 3 * x : sqrt_3_b_3 * x - 1.f / 3 * y;
      const auto r = is_flat_top ? -1.f / 3 * x + sqrt_3_b_3 * y : 2.f / 3 * y;

      return hexagon_coordinates{q, r};
    }

    [[nodiscard]] std::set<hexagon> get_hexes() const {
      return hexes;
    }

    void set_position(const glm::vec3& value) {
      position = value;
    }
  };
} // namespace hexmap
