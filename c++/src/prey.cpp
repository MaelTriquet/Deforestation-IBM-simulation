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
    Animal(parent_1_->position + sf::Vector2f(Random::rand()*2-1, Random::rand()*2-1), id_)
{
    color = sf::Color(0, 0, 255);
    max_ray_angle = PREY_MAX_RAY_ANGLE;
    is_pred = false;
    is_prey = true;
    max_velocity = PREY_MAX_VELOCITY;
    brain.delete_content();
    if (Random::rand() < .5)
        brain = Brain(parent_1_->brain, parent_2_->brain);
    else 
        brain = Brain(parent_2_->brain, parent_1_->brain);
    brain.mutate();

};


void Prey::eat() {
    if (in_tree->nb_fruit <= 0) return;
    in_tree->nb_fruit--;
    energy += FRUIT_ENERGY;
}

Prey* Prey::reproduce(Prey* parent, int id) {
    parent->energy -= LOST_ENERGY_REPRODUCTION;
    energy -= LOST_ENERGY_REPRODUCTION;
    parent->reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    Prey* child = new Prey(this, parent, id);
    child->reproduction_timeout = REPRODUCTION_TIMEOUT * (Random::rand()/2 + .75);
    return child;
};

void Prey::fight(Animal* predator) {
    predator->energy -= PRED_LOST_ENERGY_FIGHT_BY_PREY;
}
