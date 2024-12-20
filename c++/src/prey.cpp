#include "prey.hpp"

Prey::Prey(sf::Vector2f position_, int index_) : 
    Animal(position_, index_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = PREY_MAX_RAY_ANGLE;
    is_pred = false;
    is_prey = true;
    max_velocity = PREY_MAX_VELOCITY;
};

Prey::Prey(Prey* parent_1_, Prey* parent_2_, int id_) : 
    Animal(parent_1_->position, id_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = PREY_MAX_RAY_ANGLE;
    is_pred = false;
    is_prey = true;
    max_velocity = PREY_MAX_VELOCITY;

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


void Prey::eat(Tree* tree) {

}

Prey* Prey::reproduce(Prey* parent, int id) {
    parent->energy -= LOST_ENERGY_REPRODUCTION;
    energy -= LOST_ENERGY_REPRODUCTION;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT;
    reproduction_timeout = REPRODUCTION_TIMEOUT;
    Prey* child = new Prey(this, parent, id);
    child->reproduction_timeout = REPRODUCTION_TIMEOUT;
    return child;
};