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
    Animal(parent_1_->position + sf::Vector2f(Random::rand()*2-1, Random::rand()*2-1), id_)
{
    color = sf::Color(255, 0, 0);
    max_ray_angle = PRED_MAX_RAY_ANGLE;
    is_pred = true;
    is_prey = false;
    max_velocity = PRED_MAX_VELOCITY;
    // energy = parent_1_->energy + parent_2_->energy;
    // energy /= 2;
    // if (energy > INITIAL_ENERGY)
    //     energy = INITIAL_ENERGY;

    // for (int i = 0; i < brain.neurons.size(); i++) {
    //     for (int j = 0; j < brain.neurons[i].weights.size(); j++) {
    //         if (Random::rand() < 0.5) {
    //             brain.neurons[i].weights[j] = parent_1_->brain.neurons[i].weights[j];
    //         } else {
    //             brain.neurons[i].weights[j] = parent_2_->brain.neurons[i].weights[j];
    //         }
    //     }
    // }
    brain.delete_content();
    if (Random::rand() < .5)
        brain = Brain(parent_1_->brain, parent_2_->brain);
    else 
        brain = Brain(parent_2_->brain, parent_1_->brain);
    brain.mutate();
};

void Predator::eat(Animal* prey) {
    energy += PRED_GAIN_ENERGY_EATING;
    if (reproduction_timeout > 1000)
        reproduction_timeout = 10 - (REPRODUCTION_TIMEOUT - reproduction_timeout);
    // sets the prey rotting to 0 to remove it from m_pop next check
    prey->rotting -= 5;
}

void Predator::fight(Animal* prey) {
    prey->health -= PREY_LOST_ENERGY_FIGHT;
}

Predator* Predator::reproduce(Predator* parent, int id) {
    parent->energy -= LOST_ENERGY_REPRODUCTION;
    energy -= LOST_ENERGY_REPRODUCTION;
    Predator* child = new Predator(this, parent, id);
    reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    child->reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    return child;
};