#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>
#include "tree.hpp"

#define RADIUS 10
#define NB_RAY 5
#define RAY_LENGTH 50
#define ROT_TIME 30
#define INVISIBILITY_TIME 30
#define PRED_EATING_ENERGY 10
#define PREY_LOST_ENERGY_FIGHT 2


struct Vision {
    int energy;
    int fleeing;
    sf::Vector2f velocity;
    float rays[NB_RAY * 3];

    Vision():
        energy{0},
        fleeing{0},
        velocity{0,0}
    {
        std::memset(rays, 0, NB_RAY * 3 * sizeof(float));
    }

    Vision(const Vision& to_copy): 
        energy{to_copy.energy}, 
        fleeing{to_copy.fleeing}, 
        velocity{to_copy.velocity}
    {
        std::memcpy(rays, to_copy.rays, NB_RAY * 3 * sizeof(float));
    }
};

class Animal {
public:
    int radius = RADIUS;
    int energy;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool is_dead;
    sf::Color color;
    int fleeing = 0;
    Vision vision;
    float max_ray_angle;
    int index;
    bool is_pred;
    bool is_prey;
    bool is_colliding = false;
    bool is_in_tree = false;
    const Tree* in_tree = nullptr; 
    int rotting = ROT_TIME;
    int invisible = 0;
    float max_velocity;
    float max_vel_percent = .6;

    Animal(sf::Vector2f position_, sf::Vector2f velocity_, int index_);

    void considerate_bounds(int window_width, int window_height);

    void move(int window_width, int window_height);
    void reproduce(Animal* animal);
    void die();
    void look();
    void update();
};
