#include "system_manager.h"
#include <numeric>
#include <entt/signal/dispatcher.hpp>

namespace ecs {
  system_manager::system_manager(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {
    this->dispatcher.sink<event::game_quit_event>().connect<&system_manager::handle_game_quit_event>(*this);
  }

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

    std::erase_if(systems, [this](auto&& ptr) {
      return destroy_all || ptr->is_marked_to_destroy();
    });
  }

  bool system_manager::has_systems() const noexcept {
    return !systems.empty();
  }

  void system_manager::handle_game_quit_event(event::game_quit_event event) noexcept {
    destroy_all = true;
  }
} // namespace ecs