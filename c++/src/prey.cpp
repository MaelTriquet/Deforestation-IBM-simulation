#include "prey.hpp"

Prey::Prey(int energy_, sf::Vector2f position_, sf::Vector2f velocity_) : 
    Animal(energy_, position_, velocity_)
{
    color = sf::Color(0, 0, 255);
};