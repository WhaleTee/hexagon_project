#include "hexagon.h"
#include "SDL3/SDL_stdinc.h"
#include "hexmath.h"
#include <cmath>
#include <stdexcept>

hex::hexagon::hexagon(const bool& flat_top, const hex_cube_coords& cords, const float& size, const float& height) noexcept :
    coordinates{cords}, size{size}, height{height}, flat_top{flat_top} {}

[[nodiscard]] glm::vec3 hex::hexagon::get_point(const int i) const {
  if (i < 0 || i >= vertex_count) throw std::out_of_range("hexagon has 6 vertices, it's indexes could not be less than 0 and more than 5.");

  const float& degree = 60.f * static_cast<float>(i) - (flat_top ? 0 : 30.f);
  const float& radians = SDL_PI_F / 180.f * degree;

  return {size * std::cos(radians), size * std::sin(radians), 0};
}

[[nodiscard]] std::vector<glm::vec3> hex::hexagon::get_vertices() const noexcept {
  std::vector<glm::vec3> vertices{static_cast<std::size_t>(vertex_count)};

  for (int i = 0; i < vertex_count; i++) {
    vertices[i] = get_point(i);
  }

  return vertices;
}

[[nodiscard]] glm::vec3 hex::hexagon::get_local_position() const noexcept {
  const auto& qf = static_cast<float>(coordinates.q);
  const auto& rf = static_cast<float>(coordinates.r);

  if (flat_top) return {3.f / 2 * qf * size, (sqrtf(3) / 2 * qf + sqrtf(3) * rf) * size, 0};

  return {(sqrtf(3) * qf + sqrtf(3) / 2 * rf) * size, 3.f / 2 * rf * size, 0};
}

hex::hex_cube_coords hex::hexagon::get_coords() const noexcept {
  return coordinates;
}

float hex::hexagon::get_size() const noexcept {
  return size;
}

float hex::hexagon::get_height() const noexcept {
  return height;
}

bool hex::hexagon::is_flat_top() const noexcept {
  return flat_top;
}

bool hex::hexagon::operator==(const hexagon& other) const noexcept {
  return get_local_position() == other.get_local_position();
}

bool hex::hexagon::operator<(const hexagon& other) const noexcept {
  return hexmath::distance(this->coordinates, zero_hex_cube_coords) < hexmath::distance(other.coordinates, zero_hex_cube_coords);
}