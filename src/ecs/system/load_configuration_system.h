#pragma once
#include "base_system.h"

namespace ecs::system {
  class load_configuration_system final : public system {
  public:
    ~load_configuration_system() noexcept override;
    bool update() noexcept override;
    void post_update() noexcept override;
  };
}