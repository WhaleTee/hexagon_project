#pragma once
#include "../ecs/base_system.h"

namespace transform::system {
  class model_matrix_system final : public ecs::system::base_system {
    bool initialized{false};

  public:
    explicit model_matrix_system(entt::registry& registry) : base_system(registry) {}
    bool update() noexcept override;
  };
}