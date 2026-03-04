#include <cmath>
#include <complex>
#include <fstream>

#include "SDL3_ttf/SDL_ttf.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

constexpr float PI_F = 3.14;

// Rotate around Y-axis
glm::vec3 rotateY(const glm::vec3& v, float angle) {
  float rad = angle * PI_F / 180.0f;
  float cosA = cos(rad);
  float sinA = sin(rad);
  return {v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA};
}

// Rotate around X-axis
glm::vec3 rotateX(const glm::vec3& v, float angle) {
  float rad = angle * PI_F / 180.0f;
  float cosA = cos(rad);
  float sinA = sin(rad);
  return {v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA};
}

glm::vec2 iso_project(const glm::vec3& v) {
  return {v.x - v.y, (v.x + v.y) / 2 - v.z};
}

glm::vec2 iso_project(const glm::vec2& v, float z) {
  return {v.x - v.y, (v.x + v.y) / 2 - z};
}

static TTF_Font* font;

void getFont() {
  std::ifstream in{"resources/pixelLetters.ttf", std::ios::ate};
  std::ifstream inRead{"resources/pixelLetters.ttf"};
  auto size = in.tellg();
  std::string data{std::istreambuf_iterator<char>{inRead.rdbuf()}, std::istreambuf_iterator<char>()};
  font = TTF_OpenFontIO(SDL_IOFromConstMem(data.c_str(), size), true, 18.0f);
}

// int main() {
//     std::vector<std::pair<int, int>> edges = {
//         {0,1},{1,2},{2,3},{3,4},{4,5},{5,0},
//         {6,7},{7,8},{8,9},{9,10},{10,11},{11,6},
//         {0,6},{1,7},{2,8},{3,9},{4,10},{5,11}
//     };
//
//     hexmap::Hexmap map(5, 50, 25);
//     auto hexVertices = map.getVertices();
//     glm::vec2 center(1366.f/2, 768.f/2);
//     sf::RenderWindow window(sf::VideoMode({1366, 768}), "SFML works!");
//     const sf::Font &font = getFont();
//     while (window.isOpen()) {
//         while (const std::optional event = window.pollEvent()) {
//             if (event->is<sf::Event::Closed>())
//                 window.close();
//         }
//
//         window.clear();
//         sf::Text text{font, ".", 60};
//         sf::Text index{font, "", 20};
//         glm::vec2 offset = {0, -map.getHexHeight() * 3};
//         for (int i = -3; i < 4; i++) {
//             for (int j = -3; j < 4; j++) {
//                 auto worldPosition = map.hexToWorldPosition({i, j, -i - j});
//                 text.setPosition(center + iso_project(worldPosition) + offset);
//                 // index.setString("{ q: " + std::to_string(i) + ", r: " + std::to_string(j) + ", s: " + std::to_string(-i-j) + " }");
//                 // index.setPosition(text.getPosition() + offset);
//                 window.draw(text);
//                 // window.draw(index);
//
//
//                 // for (auto & edge : edges) {
//                 //     sf::Vertex line[2];
//                 //     line[0].position = center + iso_project(hexVertices[edge.first] + worldPosition);
//                 //     line[1].position = center + iso_project(hexVertices[edge.second] + worldPosition);
//                 //     window.draw(line, 2, sf::PrimitiveType::Lines);
//                     // if (i < 12) {
//                     //     sf::Text text{font, std::to_string(i)};
//                     //     text.setPosition(line[0].position);
//                     //     window.draw(text);
//                     // }
//                 // }
//             }
//         }
//         window.display();
//     }
//
//     return 0;
// }