#pragma once
#include "../ecs/base_system.h"
#include <entt/entity/registry.hpp>

namespace rendering::system {
  class begin_copy_pass final : public ecs::system::base_system {
  public:
    explicit begin_copy_pass(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

  class end_copy_pass final : public ecs::system::base_system {
  public:
    explicit end_copy_pass(entt::registry& registry) : base_system(registry) {}

    bool update() noexcept override;
  };

} // namespace rendering::system