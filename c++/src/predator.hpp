#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"

#define MAX_VELOCITY_PREDATOR 6;

class Predator : public Animal {
    public:
        sf::Color color{255, 0, 0};
        float max_velocity = MAX_VELOCITY_PREDATOR;

        Predator(int energy_, sf::Vector2f position_, sf::Vector2f velocity_);

};