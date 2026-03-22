#include "model.h"
#include "component.h"
#include "glm/gtc/quaternion.hpp"

#include <iostream>

bool transform::system::model_matrix_system::update() noexcept {
  using namespace component;

  // translation
  for (auto [entity, position, request]: registry.view<position_component, translation_request>().each()) {
    position.value += request.value;
  }

  // rotation
  constexpr auto world_right = glm::vec3{1, 0, 0};
  constexpr auto world_up = glm::vec3{0, 1, 0};
  constexpr auto world_forward = glm::vec3{0, 0, 1};

  for (auto [entity, rotation, request]: registry.view<rotation_component, rotation_request>().each()) {
    // rotation.value *= glm::angleAxis(glm::radians(request.value.x), world_right);
    // rotation.value *= glm::angleAxis(glm::radians(request.value.y), world_up);
    auto rotation_matrix = glm::mat4_cast(rotation.value);

    // std::cout << "-------- rotation start --------" << std::endl;
    // std::cout << '[' << rotation_matrix[0][0] << ", " << rotation_matrix[1][0] << ", " << rotation_matrix[2][0]<< ']' << std::endl;
    // std::cout << '[' << rotation_matrix[0][1] << ", " << rotation_matrix[1][1] << ", " << rotation_matrix[2][1]<< ']' << std::endl;
    // std::cout << '[' << rotation_matrix[0][2] << ", " << rotation_matrix[1][2] << ", " << rotation_matrix[2][2]<< ']' << std::endl;
    // std::cout << '[' << rotation_matrix[3][0] << ", " << rotation_matrix[3][1] << ", " << rotation_matrix[3][2]<< ']' << std::endl;
    // std::cout << "-------- rotation end --------" << std::endl;

    rotation.value = glm::rotate(rotation.value, glm::radians(request.value.x), glm::normalize(request.local ? rotation_matrix[0] : world_right));
    rotation.value = glm::rotate(rotation.value, glm::radians(request.value.y), glm::normalize(request.local ? rotation_matrix[1] : world_up));
    rotation.value = glm::rotate(rotation.value, glm::radians(request.value.z), glm::normalize(request.local ? rotation_matrix[2] : world_forward));
  }

  // scale
  for (auto [entity, scale, request]: registry.view<scale_component, scale_request>().each()) {
    scale.value += request.value;
  }

  // model
  for (auto [entity, model, position, rotation, scale]:registry.view<model_matrix_component, position_component, rotation_component, scale_component>().each()) {
    if (!registry.any_of<translation_request, rotation_request, scale_request>(entity) && initialized) continue;
    if (!initialized) initialized = true;

    const auto position_matrix = glm::translate(glm::mat4{1}, position.value);
    const auto rotation_matrix = glm::mat4_cast(rotation.value);
    const auto scale_matrix = glm::scale(glm::mat4{1}, scale.value);
    model.value = position_matrix * rotation_matrix * scale_matrix;

    std::cout << "-------- matrix start --------" << std::endl;
    std::cout << '[' << model.value[0][0] << ", " << model.value[0][1] << ", " << model.value[0][2] << ']' << std::endl;
    std::cout << '[' << model.value[1][0] << ", " << model.value[1][1] << ", " << model.value[1][2] << ']' << std::endl;
    std::cout << '[' << model.value[2][0] << ", " << model.value[2][1] << ", " << model.value[2][2] << ']' << std::endl;
    std::cout << '[' << model.value[3][0] << ", " << model.value[3][1] << ", " << model.value[3][2] << ']' << std::endl;
    std::cout << "-------- matrix end --------" << std::endl;

    registry.remove<translation_request>(entity);
    registry.remove<rotation_request>(entity);
    registry.remove<scale_request>(entity);
  }

  return true;
}