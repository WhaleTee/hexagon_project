#pragma once
#include "hex_cube_coords.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <cmath>

namespace hex {
  static hex_cube_coords zero_hex_cube_coords{};

  struct hexagon {
    hex_cube_coords coordinates;
    float size{1};
    float height{1};
    bool is_flat_top{true};

    explicit hexagon(const bool& flat_top, const hex_cube_coords& cords, const float& size, const float& height);

    [[nodiscard]] glm::vec3 get_point(int i) const;

    [[nodiscard]] std::vector<glm::vec3> get_vertices() const noexcept;

    [[nodiscard]] constexpr static std::vector<std::uint32_t> get_draw_line_indices() {
      return {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0};
    }

    [[nodiscard]] constexpr static std::vector<std::uint32_t> get_draw_line_strip_indices() {
      return {0, 1, 2, 3, 4, 5, 0};
    }

    [[nodiscard]] glm::vec3 get_local_position() const noexcept;

    bool operator==(const hexagon& other) const {
      return get_local_position() == other.get_local_position();
    }

    bool operator<(const hexagon& other) const {
      return this->coordinates.get_distance_to(zero_hex_cube_coords) < other.coordinates.get_distance_to(zero_hex_cube_coords);
    }
  };
} // namespace hex

template <> struct std::hash<hex::hexagon> {
  static void hash_combine(std::size_t& seed, std::size_t hash) {
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
  }

  std::size_t operator()(const hex::hexagon& hex) const noexcept {
    const auto& v = hex.get_local_position();
    constexpr std::hash<float> hasher;
    std::size_t seed = 0;
    hash_combine(seed, hasher(v.x));
    hash_combine(seed, hasher(v.y));
    hash_combine(seed, hasher(v.z));
    return seed;
  }
};