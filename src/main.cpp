#include <cmath>
#include <complex>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "hexmap.h"

constexpr float PI_F = 3.14;

// Rotate around Y-axis
sf::Vector3f rotateY(const sf::Vector3f &v, float angle) {
    float rad = angle * PI_F / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return {v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA};
}

// Rotate around X-axis
sf::Vector3f rotateX(const sf::Vector3f &v, float angle) {
    float rad = angle * PI_F / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return {v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA};
}

sf::Vector2f iso_project(const sf::Vector3f &v) {
    return {v.x - v.y, (v.x + v.y) / 2 - v.z};
}

sf::Vector2f iso_project(const sf::Vector2f &v, float z) {
    return {v.x - v.y, (v.x + v.y) / 2 - z};
}

sf::Font getFont() {
    sf::Font font;
    if (!font.openFromFile("resources/pixelLetters.ttf")) {
        std::cerr << "Could not load font\n";
    }
    return font;
}

int main() {
    std::vector<std::pair<int, int>> edges = {
        {0,1},{1,2},{2,3},{3,4},{4,5},{5,0},
        {6,7},{7,8},{8,9},{9,10},{10,11},{11,6},
        {0,6},{1,7},{2,8},{3,9},{4,10},{5,11}
    };

    hexmap::Hexmap map(5, 50, 25);
    auto hexVertices = map.getVertices();
    sf::Vector2f center(1366.f/2, 768.f/2);
    sf::RenderWindow window(sf::VideoMode({1366, 768}), "SFML works!");
    const sf::Font &font = getFont();
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        sf::Text text{font, ".", 60};
        sf::Text index{font, "", 20};
        sf::Vector2f offset = {0, -map.getHexHeight() * 3};
        for (int i = -3; i < 4; i++) {
            for (int j = -3; j < 4; j++) {
                auto worldPosition = map.hexToWorldPosition({i, j, -i - j});
                text.setPosition(center + iso_project(worldPosition) + offset);
                // index.setString("{ q: " + std::to_string(i) + ", r: " + std::to_string(j) + ", s: " + std::to_string(-i-j) + " }");
                // index.setPosition(text.getPosition() + offset);
                window.draw(text);
                // window.draw(index);


                // for (auto & edge : edges) {
                //     sf::Vertex line[2];
                //     line[0].position = center + iso_project(hexVertices[edge.first] + worldPosition);
                //     line[1].position = center + iso_project(hexVertices[edge.second] + worldPosition);
                //     window.draw(line, 2, sf::PrimitiveType::Lines);
                    // if (i < 12) {
                    //     sf::Text text{font, std::to_string(i)};
                    //     text.setPosition(line[0].position);
                    //     window.draw(text);
                    // }
                // }
            }
        }
        window.display();
    }

    return 0;
}
