#include "prey.hpp"

Prey::Prey(sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    Animal(position_, velocity_, index_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = MAX_RAY_ANGLE_PREY;
    is_pred = false;
    is_prey = true;
    max_velocity = MAX_VELOCITY_PREY;
};

Prey::Prey(Prey* parent_1_, Prey* parent_2_, int id_) : 
    Animal(parent_1_->position, sf::Vector2f{2 * Random::rand() - 1, 2 * Random::rand() - 1}, id_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = MAX_RAY_ANGLE_PREY;
    is_pred = false;
    is_prey = true;
    max_velocity = MAX_VELOCITY_PREY;

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
    parent->energy -= LOST_ENERGY_PER_REPRODUCTION;
    energy -= LOST_ENERGY_PER_REPRODUCTION;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT;
    reproduction_timeout = REPRODUCTION_TIMEOUT;
    Prey* child = new Prey(this, parent, id);
    child->reproduction_timeout = REPRODUCTION_TIMEOUT;
    return child;
};