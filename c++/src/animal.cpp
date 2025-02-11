#include "animal.hpp"

Animal::Animal(sf::Vector2f position_, int index_) : 
    position(position_),
    is_dead(false),
    index{index_},
    brain{4 + NB_RAY * 3, 2}
{
    brain.addConn();
};

//keeps all animals within the window, using a tore-like world modelisation
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

// look, think and act
void Animal::move(int window_width, int window_height) {
    look();
    brain.think(vision, decision);
    velocity = sf::Vector2f(cos(decision[0]*M_PI_2), sin(decision[0]*M_PI_2));
    velocity *= decision[1] * max_velocity;
    position += velocity;
    // energy -= decision[2] * decision[2] * max_velocity /(float)PREY_MAX_VELOCITY;
    if (is_prey)
        energy -= decision[1] * decision[1];
    else
        energy -= decision[1] * decision[1] * max_velocity + 2;
    considerate_bounds(window_width, window_height);
};

void Animal::die() {
    is_dead = (energy <= 0);
};

// fills the vision parts that don't depend on other animals
// ray vision handled in Simulation::fill_ray_visions
void Animal::look() {
    vision.energy = (float)energy / (float)MAX_ENERGY;
    vision.fleeing = fleeing;
    vision.velocity = velocity / max_velocity;
}

// updates all naturally decrementing attributes and check death
void Animal::update() {
    if (energy > 2*INITIAL_ENERGY)
        energy = 2*INITIAL_ENERGY;
    die();
    is_colliding = false;
    if (!is_in_tree)
        invisible = 0;
    else    
        invisible--;
    if (is_dead && is_pred)
        rotting = -1000;
    is_in_tree = false;
    reproduction_timeout--;
}