#pragma once
#include <SFML/Graphics.hpp>
#include "Animal.cpp"

class Predator : public Animal {
    public:
        sf::Color color{255, 0, 0};
        float max_velocity;

        Predator(int energy_, sf::Vector2f position_, sf::Vector2f velocity_) : 
        Animal(energy_, position_, velocity_)
        {}

};