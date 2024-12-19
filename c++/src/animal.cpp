#include "animal.hpp"

Animal::Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    energy(energy_),
    position(position_),
    velocity(velocity_),
    is_dead(false),
    index{index_}
{};

void Animal::considerate_bounds(int window_width, int window_height) {
    if (position.x < 0) 
        position.x += window_width;
    if (position.x >= window_width)
        position.x -= window_width;
    if (position.y < 0)
        position.y += window_height;
    if (position.y >= window_height)
        position.y -= window_height;
    
}

void Animal::move(int window_width, int window_height) {
    look();
    position += velocity;
    considerate_bounds(window_width, window_height);
};

void Animal::eat() {

};

void Animal::reproduce() {

};

void Animal::die() {
    is_dead = (energy <= 0);
};

void Animal::look() {
    vision.energy = energy;
    vision.fleeing = fleeing;
    vision.velocity = velocity;
   
    // ray vision handled in Simulation::handle_animal_interaction
}