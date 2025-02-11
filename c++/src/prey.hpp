#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"
#include "tree.hpp"
#include "const.hpp"

class Prey : public Animal {
public:

    int dead_reserve = PREY_DEAD_RESERVE;

    Prey(sf::Vector2f position_, int index_);
    Prey(Prey* parent_1_, Prey* parent_2_, int id_);

    void eat();
    Prey* reproduce(Prey* parent, int id);

};