#pragma once
#include <set>
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"


namespace hexmap {
    struct Hex {
        int q, r, s;
    };

    class Hexmap {
        unsigned int size;
        float hexSize;
        float hexHeight;
        std::set<Hex> hexes;

    public:
        Hexmap(const int size, const float hexSize, const float hexHeight): size(size), hexSize(hexSize), hexHeight(hexHeight) {}
        ~Hexmap() = default;
        [[nodiscard]] float getHexSize() const { return hexSize; }
        [[nodiscard]] float getHexHeight() const { return hexHeight; }
        [[nodiscard]] sf::Vector3f hexToWorldPosition(Hex hex) const;
        [[nodiscard]] std::vector<sf::Vector3f> getVertices() const;
    };
}
