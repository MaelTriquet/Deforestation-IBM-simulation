#pragma once
#include <SFML/Graphics.hpp>
#include "animal.hpp"
#include "maddpgAgent.hpp"
#include "const.hpp"

class Predator : public Animal {
public:
    Predator(sf::Vector2f position_, int index_);
    Predator(Predator* parent_1_, Predator* parent_2_, int id_);

    void eat(Animal* prey);
    void fight(Animal* prey);
    Predator* reproduce(Predator* parent, int id);
};
