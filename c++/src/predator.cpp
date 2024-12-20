#include "predator.hpp"

Predator::Predator(sf::Vector2f position_, sf::Vector2f velocity_, int index_) :
    Animal(position_, velocity_, index_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = MAX_RAY_ANGLE_PREDATOR;
    is_pred = true;
    is_prey = false;
    max_velocity = MAX_VELOCITY_PREDATOR;
};

Predator::Predator(Predator* parent_1_, Predator* parent_2_) : 
    Animal(parent_1_->position, sf::Vector2f{2 * Random::rand() - 1, 2 * Random::rand() - 1}, Random::randint(10000))
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = MAX_RAY_ANGLE_PREDATOR;
    is_pred = true;
    is_prey = false;
    max_velocity = MAX_VELOCITY_PREDATOR;

    for (int i = 0; i < brain.neurons.size(); i++) {
        for (int j = 0; j < brain.neurons[i].weights.size(); j++) {
            if (Random::rand() < 0.5) {
                brain.neurons[i].weights[j] = parent_1_->brain.neurons[i].weights[j];
            } else {
                brain.neurons[i].weights[j] = parent_2_->brain.neurons[i].weights[j];
            }
        }
    }
};

void Predator::eat(Animal* prey) {
    energy += PRED_EATING_ENERGY;
    prey->rotting = 0;
}

void Predator::fight(Animal* prey) {
    prey->energy -= PREY_LOST_ENERGY_FIGHT;
    energy -= PRED_LOST_ENERGY_FIGHT;
    if (prey->energy <= 0) {
        prey->is_dead = true;
    }
}

Predator* Predator::reproduce(Predator* parent) {
    parent->energy -= LOST_ENERGY_PER_REPRODUCTION;
    energy -= LOST_ENERGY_PER_REPRODUCTION;
    Predator* child = new Predator(this, parent);
    reproduction_timeout = REPRODUCTION_TIMEOUT;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT;
    child->reproduction_timeout = REPRODUCTION_TIMEOUT;
    return child;
};