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
    // energy = parent_1_->energy + parent_2_->energy;
    // energy /= 2;
    // if (energy > INITIAL_ENERGY)
    //     energy = INITIAL_ENERGY;
    
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


void Prey::eat() {
    if (in_tree->nb_fruit <= 0) return;
    in_tree->nb_fruit--;
    energy += FRUIT_ENERGY;   
    reproduction_timeout = 0;
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