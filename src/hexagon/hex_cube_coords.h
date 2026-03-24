#pragma once

namespace hex {
  struct hex_cube_coords {
    int q{0}, r{0}, s{0};

    hex_cube_coords(const int& q, const int& r) : q{q}, r{r}, s{-q - r} {}

    hex_cube_coords(const int& q, const int& r, const int& s) : q{q}, r{r}, s{s} {}

    hex_cube_coords operator+(const hex_cube_coords& other) const {
      return hex_cube_coords{this->q + other.q, this->r + other.r, this->s + other.s};
    }

    hex_cube_coords& operator+=(const hex_cube_coords& other) {
      this->q += other.q;
      this->r += other.r;
      this->s += other.s;
      return *this;
    }
  };

  static hex_cube_coords zero_hex_cube_coords{0, 0};
} // namespace hex