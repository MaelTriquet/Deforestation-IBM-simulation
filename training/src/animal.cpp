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
torch::Tensor Animal::move(int window_width, int window_height, Actor actor, torch::Tensor obs) {

    // look
    look();

    auto action = actor->forward(obs);

    std::cout << "Action: " << action << std::endl;

    // Extraire les deux valeurs
    auto angle_norm = action[0][0].item<float>();  // Valeur entre -1 et 1
    auto percent = action[0][1].item<float>();       // Valeur entre -1 et 1

    // Transformer l'angle en une plage [-π, π]
    float angle = angle_norm * M_PI;

    // Calculer la vélocité réelle en multipliant par la vitesse max
    float vitesse_max = max_velocity;
    float vitesse = percent * vitesse_max;

    position += sf::Vector2f(std::cos(angle) * vitesse, std::sin(angle) * vitesse);
    
    // loose energy
    if (energy > 0) {
        if (is_prey) {
            energy -= decision[1] * decision[1] * max_velocity + PREY_PASSIVE_ENERGY_LOSS;
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

    return action;
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
        rotting = 0;
    if (is_dead) {
        rotting -= ROTTING_PER_FRAME;
        radius = rotting / (float) ROT_TIME * ANIMALS_RADIUS * RADIUS_PERCENT_ROTTING;
    }

    is_in_tree = false;

    reproduction_timeout--;
}


