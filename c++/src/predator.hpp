#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"

#define MAX_VELOCITY_PREDATOR 6
#define MAX_RAY_ANGLE_PREDATOR M_PI

class Predator : public Animal {
public:

    Predator(sf::Vector2f position_, sf::Vector2f velocity_, int index_);
    Predator(Predator* parent_1_, Predator* parent_2_);

    void eat(Animal* prey);
    void fight(Animal* prey);
    Predator* reproduce(Predator* parent);
};