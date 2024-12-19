#include "prey.hpp"

Prey::Prey(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    Animal(energy_, position_, velocity_, index_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = MAX_RAY_ANGLE_PREY;
    is_pred = false;
    is_prey = true;
};

void Prey::eat(Tree* tree) {

}