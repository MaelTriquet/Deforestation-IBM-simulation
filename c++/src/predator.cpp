#include "predator.hpp"

Predator::Predator(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) :
    Animal(energy_, position_, velocity_, index_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = MAX_RAY_ANGLE_PREDATOR;
    is_pred = true;
    is_prey = false;
};

void Predator::eat(Animal* prey) {
    energy += PRED_EATING_ENERGY;
}

void Predator::fight(Animal* prey) {
    prey->energy -= PREY_LOST_ENERGY_FIGHT;
    if (prey->energy <= 0) {
        prey->is_dead = true;
    }
}