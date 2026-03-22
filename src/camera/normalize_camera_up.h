#pragma once
#include "../ecs/base_system.h"

namespace camera::system {
  class normalize_camera_up_system final : public ecs::system::base_system {
  public:
    explicit normalize_camera_up_system(entt::registry& registry) : base_system(registry) {}
    bool update() noexcept override;
  };
} // namespace camera::system