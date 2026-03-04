#pragma once
#include <set>
#include <vector>
#include <glm/vec3.hpp>

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
        [[nodiscard]] glm::vec3 hexToWorldPosition(Hex hex) const;
        [[nodiscard]] std::vector<glm::vec3> getVertices() const;
    };
}
