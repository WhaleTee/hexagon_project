#pragma once

namespace ecs {
  template <typename T>
  bool system_manager::has_system() noexcept {
    constexpr auto type = T::get_system_type();
    return std::ranges::any_of(systems[type], [](auto&& ptr) {
      return ptr->get_name() == T::get_class_name();
    });
  }

  template <typename... T>
  bool system_manager::has_systems() noexcept {
    return (has_system<T>() && ...);
  }

  template <typename T>
  std::optional<std::reference_wrapper<T>> system_manager::get_system_internal() noexcept {
    constexpr auto type = T::get_system_type();
    auto iterator = std::ranges::find_if(systems[type], [](auto&& ptr) {
      return ptr->get_name() == T::get_class_name();
    });

    if (iterator != systems[type].end()) {
      auto& system = static_cast<T&>(*(*iterator));
      return std::optional{std::reference_wrapper<T>(system)};
    }
    return std::optional<std::reference_wrapper<T>>();
  }

  template <typename T>
  T& system_manager::get_system() noexcept {
    return (*get_system_internal<T>()).get();
  }

  template <typename... T>
  std::tuple<std::add_lvalue_reference_t<T>...> system_manager::get_systems() noexcept {
    return {get_system<T>()...};
  }

  template <typename T>
  bool system_manager::mark_system_to_destroy() noexcept {
    if (has_system<T>()) {
      get_system<T>().mark_to_destroy();
      marked_to_sweep = true;
      return true;
    }
    return false;
  }

  template <typename... T>
  bool system_manager::mark_systems_to_destroy() noexcept {
    return (mark_system_to_destroy<T>() && ...);
  }

  template <typename T>
  bool system_manager::enable_system() noexcept {
    if (has_system<T>()) {
      get_system<T>().enable();
      return true;
    }

    return false;
  }

  template <typename... T>
  bool system_manager::enable_systems() noexcept {
    return (enable_system<T>() && ...);
  }

  template <typename T>
  bool system_manager::disable_system() noexcept {
    if (has_system<T>()) {
      get_system<T>().disable();
      return true;
    }

    return false;
  }

  template <typename... T>
  bool system_manager::disable_systems() noexcept {
    return (disable_system<T>() && ...);
  }

  template <typename T, typename... TArgs>
  T& system_manager::create_system(TArgs&&... args) noexcept {
    if (has_system<T>()) return get_system<T>();
    return static_cast<T&>(add_system_internal(std::move(std::make_unique<T>(this->registry, std::forward<TArgs>(args)...)), T::get_system_type()));
  }

  template <typename T, typename... TArgs>
  void system_manager::create_system_at_runtime(TArgs&&... args) noexcept {
    if (has_system<T>()) return;

    this->dispatcher.trigger<event::add_base_system_event>(std::move(std::make_unique<T>(this->registry, std::forward<TArgs>(args)...)));
  }
} // namespace ecs