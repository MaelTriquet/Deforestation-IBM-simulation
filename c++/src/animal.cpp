#include "animal.hpp"

// constructor
Animal::Animal(sf::Vector2f position_, int index_) : 
    position(position_),
    is_dead(false),
    brain{4 + NB_RAY * 2, 2},
    index{index_}
{
    brain.addConn();
};

// keeps all animals within the window, using a tore-like world modelisation
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

    // look
    look();

    // think 
    brain.think(vision, decision);

    // act
    velocity = sf::Vector2f(cos(decision[0]*M_PI_2), sin(decision[0]*M_PI_2));
    if (decision[1] > 1)
        velocity *= max_velocity;
    else if (decision[1] < -1)
        velocity *= -max_velocity;
    else 
        velocity *= decision[1] * max_velocity;
    position += velocity;
    
    // loose energy
    if (energy > 0) {
        if (is_prey) {
            energy -= decision[1] * decision[1] * max_velocity + PREY_PASSIVE_ENERGY_LOSS; // ici
        }
        else {
            energy -= decision[1] * decision[1] * max_velocity + PRED_PASSIVE_ENERGY_LOSS;
        }
    } else {
        if (is_prey) {
            health -= decision[1] * decision[1] * max_velocity + PREY_PASSIVE_ENERGY_LOSS;
        }
        else {
            health -= decision[1] * decision[1] * max_velocity + PRED_PASSIVE_ENERGY_LOSS;
        }
    }

    // stay in the tore-like world
    considerate_bounds(window_width, window_height);
};

// an animal dies if its health reaches 0
void Animal::die() {
    is_dead = (health <= 0);
};

// fills the vision parts that don't depend on other animals
// ray vision handled in Simulation::fill_ray_visions
void Animal::look() {
    vision.energy = energy / (float)MAX_ENERGY;
    vision.health = health / (float)MAX_ENERGY;
    vision.fleeing = fleeing;
    vision.velocity.x = brain.neurons[brain.neurons.size()-2]->value;
    vision.velocity.y = brain.neurons[brain.neurons.size()-1]->value;
}

// check is the animal sees another animal
bool Animal::has_in_rays(Animal* animal) {
    bool in_rays = false;
    for (int i = 0; i < NB_RAY; i++) {
        if ((is_pred && !animal->is_pred && vision.rays[i] > 0 && vision.rays[i+NB_RAY] > 0) || (is_prey && vision.rays[i] > 0 && vision.rays[i+NB_RAY] < 0)) {
            in_rays = true;
            break;
        }
    }
    return in_rays;
}

// updates all naturally decrementing attributes and check death
void Animal::update() {

    if (energy > MAX_ENERGY)
        energy = MAX_ENERGY;
    
    die();

    is_colliding = false;

    if (!is_in_tree)
        invisible = 0;
    else
        invisible--;
    is_in_tree = false;

    if (is_dead && is_pred)
        rotting = -1000;
    if (is_dead) {
        rotting -= .5;
        radius =  rotting / (float) ROT_TIME * ANIMALS_RADIUS * .7;
    }

    is_in_tree = false;

    reproduction_timeout--;
}