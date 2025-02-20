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
    // Supprime l'ancien réseau
    delete agent;

    // Crée un nouvel agent basé sur l'un des parents
    if (Random::rand() < 0.5)
        agent = new MADDPGAgent(*parent_1_->agent); 
    else
        agent = new MADDPGAgent(*parent_2_->agent);

    // Mutation de l'agent
    agent->mutate();
};

void Predator::eat(Animal* prey) {
    energy += PRED_GAIN_ENERGY_EATING;
    reproduction_timeout = 0;
    // sets the prey rotting to 0 to remove it from m_pop next check
    prey->rotting = 0;
}

void Predator::fight(Animal* prey) {
    reward += 30;
    prey->reward -= 30;
    float a = (float)prey->energy * PRED_PERCENT_DAMAGE;
    prey->energy -= PREY_LOST_ENERGY_FIGHT > a ? PREY_LOST_ENERGY_FIGHT : a;
}

Predator* Predator::reproduce(Predator* parent, int id) {
    reward += 100;
    parent->reward += 100;
    parent->energy -= LOST_ENERGY_REPRODUCTION;
    energy -= LOST_ENERGY_REPRODUCTION;
    Predator* child = new Predator(this, parent, id);
    reproduction_timeout = REPRODUCTION_TIMEOUT;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT;
    child->reproduction_timeout = REPRODUCTION_TIMEOUT;
    return child;
};