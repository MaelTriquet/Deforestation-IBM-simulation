#include "animal.hpp"

Animal::Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    energy(energy_),
    position(position_),
    velocity(velocity_),
    is_dead(false),
    index{index_}
{};

sf::Vector2f Animal::considerate_bounds(sf::Vector2f theoretical_position, int window_width, int window_height) {
    if (theoretical_position.x < 0) {
        theoretical_position.x += window_width;
    }
    if (theoretical_position.x > window_width) {
        theoretical_position.x -= window_width;
    }
    if (theoretical_position.y < 0) {
        theoretical_position.y += window_height;
    }
    if (theoretical_position.y > window_height) {
        theoretical_position.y -= window_height;
    }
    return(theoretical_position);
}

void Animal::move(int window_width, int window_height) {
    position += velocity;
    position = considerate_bounds(position, window_width, window_height);
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