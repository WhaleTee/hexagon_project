#include "hexmap.h"

#include <cmath>

namespace hexmap {

    sf::Vector3f Hexmap::hexToWorldPosition(Hex hex) const {
        return {
            (sqrtf(3) * static_cast<float>(hex.q) + sqrtf(3) / 2 * static_cast<float>(hex.r)) * hexSize,
            3.f/2 * static_cast<float>(hex.r) * hexSize,
            hexHeight / 2
        };
    }

    std::vector<sf::Vector3f> Hexmap::getVertices() const {
        auto half_width = static_cast<float>(sqrt(3) * hexSize) / 2;
        auto half_size = hexSize / 2.f;
        return {
            {half_width,half_size,hexHeight}, {0,hexSize,hexHeight}, {-half_width,half_size,hexHeight},
            {-half_width,-half_size,hexHeight}, {0,-hexSize,hexHeight}, {half_width,-half_size,hexHeight},
            {half_width,half_size,-half_size}, {0,hexSize,-hexHeight}, {-half_width,half_size,-hexHeight},
            {-half_width,-half_size,-hexHeight}, {0,-hexSize,-hexHeight}, {half_width,-half_size,-hexHeight},
        };
    }
}
