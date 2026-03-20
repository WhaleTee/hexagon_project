#include "hex_cube_coords.h"
#include "hexmath.h"

hex::hex_cube_coords::hex_cube_coords(const int& q, const int& r) : q{q}, r{r}, s{-q - r} {}

hex::hex_cube_coords::hex_cube_coords(const float& q, const float& r) {
  *this = hexmath::round_coordinates(q, r);
}

hex::hex_cube_coords::hex_cube_coords(const int& q, const int& r, const int& s) : q{q}, r{r}, s{s} {}

hex::hex_cube_coords::hex_cube_coords(const float& q, const float& r, const float& s) {
  *this = hexmath::round_coordinates(q, r, s);
}

std::size_t hex::hex_cube_coords::get_distance_to(const hex_cube_coords& coords) const noexcept {
  return hexmath::distance(*this, coords);
}