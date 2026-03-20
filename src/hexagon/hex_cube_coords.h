#pragma once
#include <cstddef>

namespace hex {
  struct hex_cube_coords {
    int q{0}, r{0}, s{0};

    hex_cube_coords() = default;

    hex_cube_coords(const int& q, const int& r);

    hex_cube_coords(const float& q, const float& r);

    hex_cube_coords(const int& q, const int& r, const int& s);

    hex_cube_coords(const float& q, const float& r, const float& s);

    [[nodiscard]] std::size_t get_distance_to(const hex_cube_coords& coords) const noexcept;

    hex_cube_coords operator+(const hex_cube_coords& other) const {
      return {this->q + other.q, this->r + other.r, this->s + other.s};
    }

    hex_cube_coords& operator+=(const hex_cube_coords& other) {
      this->q += other.q;
      this->r += other.r;
      this->s += other.s;
      return *this;
    }
  };
} // namespace hex