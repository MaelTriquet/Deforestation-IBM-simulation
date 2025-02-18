#include "animal.hpp"
#include "const.hpp"


// Constructeur de l'Animal
Animal::Animal(sf::Vector2f position_, int index_) :
    position(position_),
    index(index_),
    is_dead(false),
    agent(new MADDPGAgent(6 + NB_RAY * 3, 2, 0.001, 0.001))  // Initialisation correcte de l'agent MADDPG
{
    velocity = sf::Vector2f(0, 0);
    energy = INITIAL_ENERGY;
    max_velocity = is_prey ? PREY_MAX_VELOCITY : PRED_MAX_VELOCITY;
}

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
    torch::Tensor action = agent->select_action(get_state());
    action = action.squeeze(0);
    
    velocity = sf::Vector2f(cos(action[0].item<float>() * M_PI_2), sin(action[0].item<float>() * M_PI_2));
    velocity *= action[1].item<float>() * max_velocity;

    position += velocity;
    if (is_prey)
        energy -= decision[1] * decision[1];
    else
        energy -= decision[1] * decision[1] * max_velocity + 10;
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
    if (is_dead)
        rotting--;
    if (is_dead && is_pred)
        rotting = -1000;
    is_in_tree = false;
    reproduction_timeout--;
}
//Gets all the data needed for the maddpg brain
torch::Tensor Animal::get_state() const {
    std::vector<float> state_vector;
    state_vector.push_back(static_cast<float>(energy) / MAX_ENERGY);
    state_vector.push_back(velocity.x / max_velocity);
    state_vector.push_back(velocity.y / max_velocity);
    state_vector.push_back(static_cast<float>(fleeing));
    state_vector.push_back(static_cast<float>(invisible) / INVISIBILITY_TIME);
    state_vector.push_back(static_cast<float>(reproduction_timeout) / REPRODUCTION_TIMEOUT);
    
    for (int i = 0; i < NB_RAY * 3; i++) {
        state_vector.push_back(vision.rays[i]);
    }
    
    return torch::tensor(state_vector, torch::dtype(torch::kFloat32)).unsqueeze(0);

}