#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace camera::system {
  class view_matrix_system final : public ecs::system::base_system {
  public:
    explicit view_matrix_system(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };
} // namespace space::system