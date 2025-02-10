#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include "const.hpp"

struct Vision {
    float energy;
    float fleeing;
    sf::Vector2f velocity;
    float rays[NB_RAY * 3];

    Vision():
        energy{0.f},
        fleeing{0.f},
        velocity{0,0}
    {
        std::memset(rays, 0.f, NB_RAY * 3 * sizeof(float));
    }

    Vision(const Vision& to_copy):
        energy{to_copy.energy}, 
        fleeing{to_copy.fleeing}, 
        velocity{to_copy.velocity}
    {
        std::memcpy(rays, to_copy.rays, NB_RAY * 3 * sizeof(float));
    }
};