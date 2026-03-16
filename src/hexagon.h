#pragma once
#include "glm/detail/func_exponential.inl"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <array>
#include <cmath>
#include <stdexcept>

struct hexagon_coordinates {
  int q{0}, r{0}, s{0};

  explicit hexagon_coordinates(const int q, const int r) : q{q}, r{r}, s{-q - r} {}

  explicit hexagon_coordinates(const float q, const float r) {
    this->round_coordinates(q, r);
  }

  explicit hexagon_coordinates(const int q, const int r, const int s) : q{q}, r{r}, s{s} {}

  explicit hexagon_coordinates(const float q, const float r, const float s) {
    this->round_coordinates(q, r, s);
  }

  void round_coordinates(const float q, const float r, float s = 0) {
    if (s <= 0) s = -q - r;

    auto q_round = roundf(q);
    auto r_round = roundf(r);
    auto s_round = roundf(s);
    const auto q_delta = abs(q_round - q);
    const auto r_delta = abs(r_round - r);
    const auto s_delta = abs(s_round - s);

    if (q_delta > r_delta && q_delta > s_delta) q_round = -r_round - s_round;
    else if (r_delta > s_delta) r_round = -q_round - s_round;
    else s_round = -q_round - r_round;

    this->q = static_cast<int>(q_round);
    this->r = static_cast<int>(r_round);
    this->s = static_cast<int>(s_round);
  }
};

struct hexagon {
  hexagon_coordinates coordinates;
  float size{1};
  float height{1};
  bool is_flat_top{true};

  explicit hexagon(const bool flat_top, const int q, const int r, const float size, const float height) :
      is_flat_top{flat_top}, coordinates{q, r}, size{size}, height{height} {}


  [[nodiscard]] glm::vec3 get_point(const int i) const {
    if (i < 0 || i > 11) throw std::out_of_range("in 3D space hexagon has 12 vertices, it's indexes could not be less than 0 and more than 11.");
    const float degree = 60.f * static_cast<float>(i) - (is_flat_top ? 0 : 30.f);
    const float radians = glm::pi<float>() / 108 * degree;
    return {size * glm::cos(degree), size * glm::sin(radians), i < 6 ? 0 : height};
  }

  [[nodiscard]] std::array<glm::vec3, 12> get_vertices() const {
    std::array<glm::vec3, 12> vertices{};
    for (int i = 0; i < 12; i++) {
      vertices[i] = get_point(i);
    }
    return vertices;
  }

  [[nodiscard]] glm::vec3 get_world_position() const {
    const auto qf = static_cast<float>(coordinates.q);
    const auto rf = static_cast<float>(coordinates.r);
    if (is_flat_top) return {3.f / 2 * qf * size, (sqrtf(3) / 2 * qf + sqrtf(3) * rf) * size, 0};
    return {(sqrtf(3) * qf + sqrtf(3) / 2 * rf) * size, 3.f / 2 * rf * size, height / 2};
  }
};