#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"
#include "tree.hpp"

#define MAX_VELOCITY_PREY 4
#define MAX_RAY_ANGLE_PREY M_PI

class Prey : public Animal {
public:
    float max_velocity = MAX_VELOCITY_PREY;

    Prey(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_);

    void eat(Tree* tree);

};