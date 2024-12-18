#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>

#define RADIUS 10
#define NB_RAY 5

class Animal {
public:
    int radius = RADIUS;
    int energy;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool is_dead;
    sf::Color color;
    int fleeing = 0;
    bool is_collisioning = false;

    Vision vision;

    Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_);

    sf::Vector2f considerate_bounds(sf::Vector2f theoretical_position, int window_width, int window_height);

    void move(int window_width, int window_height);
    void eat();
    void reproduce();
    void die();
    void look();
};

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