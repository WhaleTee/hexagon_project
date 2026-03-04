#include "system_manager.h"

#include "add_base_system_event.h"

#include <entt/signal/dispatcher.hpp>
#include <numeric>
#include <ranges>

namespace ecs {
  system_manager::system_manager(entt::registry& registry, entt::dispatcher& dispatcher) noexcept : registry(registry), dispatcher(dispatcher) {
    dispatcher.sink<event::add_base_system_event>().connect<&system_manager::on_add_base_system_event>(*this);
  }

  system_manager::~system_manager() noexcept = default;

  base_system& system_manager::add_system_internal(system_ptr&& system, const system_type type) noexcept {
    return *systems[type].emplace_back(std::move(system));
  }

  void system_manager::sweep_systems_internal() noexcept {
    std::ranges::for_each(systems, [](auto& systems) {
      std::ranges::remove_if(systems, &base_system::is_marked_to_destroy);
    });
    marked_to_sweep = false;
  }

  void system_manager::start() noexcept {
    game_is_running = true;
  }

  std::size_t system_manager::update() noexcept {
    if (!get_systems_count() || !game_is_running) return 0;

    std::size_t updated_systems{0};

    updated_systems += update_systems(system_type::initialization);
    updated_systems += update_systems(system_type::update);
    updated_systems += update_systems(system_type::post_update);

    if (marked_to_sweep) sweep_systems_internal();

    for (auto&& vector: systems) {
      for (auto&& system: vector) {
        system->post_update();
      }
    }

    if (!add_system_requests.empty()) {
      while (!add_system_requests.empty()) {
        const auto type = add_system_requests.front()->get_system_type_rtti();
        add_system_internal(std::move(add_system_requests.front()), type);
        add_system_requests.pop();
      }
    }

    return updated_systems;
  }

  std::size_t system_manager::update_systems(const system_type type) noexcept {
    std::size_t updated_systems{0};

    for (auto&& system: systems[type] | std::ranges::views::filter(&base_system::is_enabled)) {
      system->update();
      updated_systems++;
    }

    return updated_systems;
  }

  std::size_t system_manager::get_systems_count() const noexcept {
    return std::accumulate(systems.begin(), systems.end(), 0uLL, [](const auto accumulator, auto&& vector) {
      return accumulator + vector.size();
    });
  }

  std::size_t system_manager::get_systems_count(const system_type type) const noexcept {
    return systems[type].size();
  }

  void system_manager::on_add_base_system_event(const event::add_base_system_event& event) noexcept {
    const auto type = event.system_ptr->get_system_type_rtti();

    if (!game_is_running) {
      add_system_internal(std::move(const_cast<system_ptr&>(event.system_ptr)), type);
    } else {
      add_system_requests.push(std::move(const_cast<system_ptr&>(event.system_ptr)));
    }
  }

  system_manager& system_manager::operator+=(system_ptr system) noexcept {
    const auto type = system->get_system_type_rtti();
    add_system_internal(std::move(system), type);
    return *this;
  }
} // namespace ecs