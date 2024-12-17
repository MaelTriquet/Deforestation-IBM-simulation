#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"

#define MAX_VELOCITY_PREY 4;

class Prey : public Animal {
    public:
        float max_velocity = MAX_VELOCITY_PREY;

        Prey(int energy_, sf::Vector2f position_, sf::Vector2f velocity_);

};