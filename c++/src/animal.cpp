#include "animal.hpp"

Animal::Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_) : 
    energy(energy_),
    position(position_),
    velocity(velocity_),
    is_dead(false),
    index{index_},
    brain{19, 3, 3, 10}
{
    brain.mutate();
};

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
    brain.think(vision, decision);
    std::cout << decision[0] << ", " << decision[1] << ", " << decision[2] << "\n";
    velocity = sf::Vector2f(decision[0], decision[1]);
    decision[2] = decision[2] > 1 ? 1 : decision[2];
    float vel_mag = std::sqrt(velocity.x * velocity.x + velocity.y*velocity.y);
    if (vel_mag > 0)
        velocity *= decision[2] * max_velocity / vel_mag;
    position += velocity;
    energy -= decision[2] * decision[2];
    considerate_bounds(window_width, window_height);
};

void Animal::reproduce(Animal* animal) {
    
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
}