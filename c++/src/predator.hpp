#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"

#define MAX_VELOCITY_PREDATOR 6;

class Predator : public Animal {
    public:
        float max_velocity = MAX_VELOCITY_PREDATOR;

        Predator(int energy_, sf::Vector2f position_, sf::Vector2f velocity_);

};