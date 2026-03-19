#pragma once
#include "../camera/component.h"
#include "../ecs/base_system.h"
#include "../rendering/render.h"
#include "glm/gtc/quaternion.hpp"
#include <entt/entity/registry.hpp>

namespace space::system {
  struct view_matrix_system final : ecs::system::base_system {
    explicit view_matrix_system(entt::registry& registry) : base_system(registry) {}

    ~view_matrix_system() noexcept override = default;

    bool update() noexcept override {
      using namespace component;
      using namespace camera::component;

      const auto& view = registry.view<orientation_component, position_component, view_matrix_component>();

      for (auto&& [entity, orientation, position, view_matrix]: view.each()) {
        auto rotation_matrix = orientation.value;
        auto translation_matrix = glm::translate(glm::mat4(1.f), -position.value);

        view_matrix.value = rotation_matrix * translation_matrix;
        // view_matrix.value = rotation_matrix;
        // std::cout << "-------- orientation start --------" << std::endl;
        // std::cout << '[' << orientation.value.x << ", " << orientation.value.y << ", " << orientation.value.z << ']' << std::endl;
        // std::cout << "-------- orientation end --------" << std::endl;
        // std::cout << "-------- matrix start --------" << std::endl;
        // std::cout << '[' << view_matrix.value[0][0] << ", " << view_matrix.value[0][1] << ", " << view_matrix.value[0][2]<< ", " << view_matrix.value[0][3] << ']' << std::endl;
        // std::cout << '[' << view_matrix.value[1][0] << ", " << view_matrix.value[1][1] << ", " << view_matrix.value[1][2]<< ", " << view_matrix.value[1][3] << ']' << std::endl;
        // std::cout << '[' << view_matrix.value[2][0] << ", " << view_matrix.value[2][1] << ", " << view_matrix.value[2][2]<< ", " << view_matrix.value[2][3] << ']' << std::endl;
        // std::cout << '[' << view_matrix.value[3][0] << ", " << view_matrix.value[3][1] << ", " << view_matrix.value[3][2]<< ", " << view_matrix.value[3][3] << ']' << std::endl;
        // std::cout << "-------- matrix end --------" << std::endl;
      }

      return true;
    }
  };
} // namespace space::system