#include "hexmap.h"
#include "hexmath.h"
#include  <cmath>

hex::hexmap::hexmap(const std::size_t& size, const float& hex_size, const float& hex_height, const bool& flat_top) :
    map_size(static_cast<int>(size)), hex_size(hex_size), hex_height(hex_height), is_flat_top(flat_top) {
  build_hexmap();
}

void hex::hexmap::build_hexmap() noexcept {
  const auto& start_hex = hexagon{is_flat_top, zero_hex_cube_coords, hex_size, hex_height};

  hexes.clear();
  hexes.reserve(hexmath::get_hexmap_hexes_count(map_size));
  hexes.insert(start_hex);

  for (auto&& hex : hexmath::construct_all_for_distance(start_hex, map_size) | std::views::join) {
    hexes.insert(hex);
  }
}

[[nodiscard]] std::unordered_set<hex::hexagon> hex::hexmap::get_hexes() const noexcept {
  return hexes;
}

[[nodiscard]] hex::hexagon hex::hexmap::get_hexagon(const glm::vec2& world_position) const noexcept {
  const float& sqrt_3_b_3 = sqrtf(3) / 3;
  const auto& x = world_position.x / hex_size;
  const auto& y = world_position.y / hex_size;
  const auto& q = is_flat_top ? 2.f / 3 * x : sqrt_3_b_3 * x - 1.f / 3 * y;
  const auto& r = is_flat_top ? -1.f / 3 * x + sqrt_3_b_3 * y : 2.f / 3 * y;

  return hexagon{is_flat_top, {q, r}, hex_size, hex_height};
}

[[nodiscard]] glm::vec3 hex::hexmap::get_hexagon_world_position(const hexagon& hex) const noexcept {
  return hex.get_local_position() + position;
}

void hex::hexmap::set_position(const glm::vec3& value) noexcept {
  position = value;
}