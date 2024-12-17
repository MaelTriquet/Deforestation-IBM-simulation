#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.cpp"

class Prey : public Animal {
    public:
        sf::Color color{0, 0, 255};
        float max_velocity;

        Prey(int energy_, sf::Vector2f position_, sf::Vector2f velocity_) : 
        Animal(energy_, position_, velocity_)
        {}

};