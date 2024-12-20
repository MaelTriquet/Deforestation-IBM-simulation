#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"
#include "tree.hpp"
#include "const.hpp"

class Prey : public Animal {
public:

    Prey(sf::Vector2f position_, int index_);
    Prey(Prey* parent_1_, Prey* parent_2_);

    void eat(Tree* tree);
    Prey* reproduce(Prey* parent);

};