#include "predator.hpp"

Predator::Predator(sf::Vector2f position_, int index_) :
    Animal(position_, index_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = PRED_MAX_RAY_ANGLE;
    is_pred = true;
    is_prey = false;
    max_velocity = PRED_MAX_VELOCITY;
};

Predator::Predator(Predator* parent_1_, Predator* parent_2_, int id_) : 
    Animal(parent_1_->position, id_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = PRED_MAX_RAY_ANGLE;
    is_pred = true;
    is_prey = false;
    max_velocity = PRED_MAX_VELOCITY;

    for (int i = 0; i < brain.neurons.size(); i++) {
        for (int j = 0; j < brain.neurons[i].weights.size(); j++) {
            if (Random::rand() < 0.5) {
                brain.neurons[i].weights[j] = parent_1_->brain.neurons[i].weights[j];
            } else {
                brain.neurons[i].weights[j] = parent_2_->brain.neurons[i].weights[j];
            }
        }
    }

    brain.mutate();
};

void Predator::eat(Animal* prey) {
    energy += PRED_GAIN_ENERGY_EATING;
    // sets the prey rotting to 0 to remove it from m_pop next check
    prey->rotting = 0;
}

void Predator::fight(Animal* prey) {
    prey->energy -= PREY_LOST_ENERGY_FIGHT;
}

Predator* Predator::reproduce(Predator* parent, int id) {
    parent->energy -= LOST_ENERGY_REPRODUCTION;
    energy -= LOST_ENERGY_REPRODUCTION;
    Predator* child = new Predator(this, parent, id);
    reproduction_timeout = REPRODUCTION_TIMEOUT;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT;
    child->reproduction_timeout = REPRODUCTION_TIMEOUT;
    return child;
};