#pragma once
#include <array>
#include <cstddef>

namespace hex {
  class hex_cube_coords {
    int q{0}, r{0}, s{0};

  public:
    hex_cube_coords() noexcept = default;

    hex_cube_coords(const int& q, const int& r) noexcept;

    hex_cube_coords(const float& q, const float& r) noexcept;

    hex_cube_coords(const int& q, const int& r, const int& s) noexcept;

    hex_cube_coords(const float& q, const float& r, const float& s) noexcept;

    [[nodiscard]] std::array<int, 3> get_qrs() const noexcept;

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

  static hex_cube_coords zero_hex_cube_coords{};
} // namespace hex