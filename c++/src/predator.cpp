#include "predator.hpp"

Predator::Predator(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) :
    Animal(energy_, position_, velocity_, index_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = MAX_RAY_ANGLE_PREDATOR;
    is_pred = true;
    is_prey = false;
    max_velocity = MAX_VELOCITY_PREDATOR;
};
