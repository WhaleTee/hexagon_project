#include "system_manager.h"
#include <numeric>

namespace ecs {
  system::base_system& system_manager::add_system(system_ptr&& system) noexcept {
    return *systems.emplace_back(std::move(system));
  }

  void system_manager::update() noexcept {
    update_systems();
  }

  void system_manager::update_systems() noexcept {
    if (systems.empty()) return;

    for (auto&& system: systems) {
      system->update();
    }

    std::erase_if(systems, [](auto&& ptr) {
      return ptr->is_marked_to_destroy();
    });
  }
} // namespace ecs