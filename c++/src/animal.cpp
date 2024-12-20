#include "animal.hpp"

Animal::Animal(sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    position(position_),
    velocity(velocity_),
    is_dead(false),
    index{index_},
    brain{19, 3, 3, 10}
{};

void Animal::considerate_bounds(int window_width, int window_height) {
    while (position.x < 0) 
        position.x += window_width;
    while (position.x >= window_width)
        position.x -= window_width;
    while (position.y < 0)
        position.y += window_height;
    while (position.y >= window_height)
        position.y -= window_height;
}

void Animal::move(int window_width, int window_height) {
    look();
    float vel_mag = std::sqrt(velocity.x * velocity.x + velocity.y*velocity.y);
    if (vel_mag > 0)
        velocity *= max_vel_percent * max_velocity / vel_mag;
    position += velocity;
    energy -= max_vel_percent * max_vel_percent - .05;
    considerate_bounds(window_width, window_height);
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

void Animal::update() {
    die();
    is_colliding = false;
    if (!is_in_tree)
        invisible = 0;
    else    
        invisible--;
    if (is_dead)
        rotting--;
    is_in_tree = false;
    reproduction_timeout--;
}