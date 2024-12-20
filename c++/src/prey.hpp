#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"
#include "tree.hpp"

#define MAX_VELOCITY_PREY 4
#define MAX_RAY_ANGLE_PREY M_PI

class Prey : public Animal {
public:

    Prey(sf::Vector2f position_, sf::Vector2f velocity_, int index_);
    Prey(Prey* parent_1_, Prey* parent_2_, int id_);

    void eat(Tree* tree);
    Prey* reproduce(Prey* parent, int id);

};