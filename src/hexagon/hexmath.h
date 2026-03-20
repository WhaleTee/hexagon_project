#pragma once
#include "hex_cube_coords.h"
#include "hexagon.h"
#include <algorithm>
#include <numeric>
#include <ranges>
#include <unordered_set>

namespace hex::hexmath {
  enum hex_cube_direction : std::size_t { R = 0, QR, Q, SQ, S, RS };

  inline const std::vector<hex_cube_coords> direction_vectors{
      {1, 0, -1}, {1, -1, 0}, {0, -1, 1}, {-1, 0, 1}, {-1, +1, 0}, {0, +1, -1},
  };

  inline std::size_t get_hexmap_hexes_count(const std::size_t& size) {
    return 6 * (size * (size + 1) / 2) + 1;
  }

  inline hex_cube_coords round_coordinates(const float& q, const float& r, float s = 0) {
    if (s <= 0) s = -q - r;

    auto q_round = std::roundf(q);
    auto r_round = std::roundf(r);
    auto s_round = std::roundf(s);
    const auto q_delta = abs(q_round - q);
    const auto r_delta = abs(r_round - r);
    const auto s_delta = abs(s_round - s);

    if (q_delta > r_delta && q_delta > s_delta) q_round = -r_round - s_round;
    else if (r_delta > s_delta) r_round = -q_round - s_round;
    else s_round = -q_round - r_round;

    return hex_cube_coords{static_cast<int>(q_round), static_cast<int>(r_round), static_cast<int>(s_round)};
  }

  inline std::size_t distance(const hex_cube_coords& a, const hex_cube_coords& b) {
    return (std::abs(a.q - b.q) + std::abs(a.q + a.r - b.q - b.r) + std::abs(a.r - b.r)) / 2;
  }

  inline std::size_t distance(const hexagon& a, const hexagon& b) {
    return distance(a.coordinates, b.coordinates);
  }

  inline std::vector<hex_cube_coords> get_neighbors(const hex_cube_coords& position) {
    std::vector result{direction_vectors};
    for (auto&& neighbor: result) {
      neighbor += position;
    }
    return result;
  }

  inline std::vector<hex_cube_coords> get_neighbors(const hexagon& hex) {
    return get_neighbors(hex.coordinates);
  }

  inline std::vector<std::vector<hex_cube_coords>> get_neighbors(const std::vector<hex_cube_coords>& positions) {
    std::vector result{positions.size(), direction_vectors};
    for (auto i = 0; i < result.size(); i++) {
      for (auto&& neighbor: result[i]) {
        neighbor += positions[i];
      }
    }
    return result;
  }

  inline std::vector<std::vector<hex_cube_coords>> get_neighbors(const std::vector<hexagon>& hexes) {
    std::vector result{hexes.size(), direction_vectors};
    for (auto i = 0; i < result.size(); i++) {
      for (auto&& neighbor: result[i]) {
        neighbor += hexes[i].coordinates;
      }
    }
    return result;
  }

  inline hex_cube_coords get_direction_neighbor(const hex_cube_coords& position, const hex_cube_direction& direction) {
    return position + direction_vectors[direction];
  }

  inline std::vector<hex_cube_coords> get_direction_neighbors(const hex_cube_coords& start, const hex_cube_direction& direction, const std::size_t& distance) {
    std::vector<hex_cube_coords> result{};

    result.reserve(distance);
    result.emplace_back(start);

    for (auto i = 0; i < result.capacity(); i++) {
      result.emplace_back(get_direction_neighbor(result[result.size() - 1], direction));
    }

    result.erase(result.begin());

    return result;
  }

  inline std::vector<std::vector<hexagon>> construct_all_for_distance(const hexagon& start, const std::size_t& distance) {
    const std::size_t size = get_hexmap_hexes_count(distance);
    std::vector<std::vector<hexagon>> result_vector{size};
    std::unordered_set<hexagon> visited{size};
    auto hex_height = start.height;
    auto hex_size = start.size;
    auto flat_top = start.is_flat_top;
    auto step = 1;
    auto emplace_neighbors = [&](const hex_cube_coords& neighbor) {
      const auto& [it, success] = visited.emplace(flat_top, neighbor, hex_size, hex_height);
      if (success) result_vector[step].emplace_back(*it);
    };

    std::ranges::for_each(get_neighbors(start), emplace_neighbors);

    while (step < distance) {
      std::ranges::for_each(get_neighbors(result_vector[step - 1]) | std::ranges::views::join, emplace_neighbors);
      step++;
    }
    return result_vector;
  }

  inline std::vector<glm::vec3> get_hexagon_vertices(const hexagon& hex) noexcept {
    std::vector<glm::vec3> vertices = hex.get_vertices();
    for (auto& vertex: vertices) {
      vertex += hex.get_local_position();
    }
    return vertices;
  }
} // namespace hex::hexmath