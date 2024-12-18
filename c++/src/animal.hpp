#pragma once
#include <SFML/Graphics.hpp>

#define RADIUS 10

class Animal {
public:
    int radius = RADIUS;
    int energy;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool is_dead;
    sf::Color color;

    Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_);

    sf::Vector2f considerate_bounds(sf::Vector2f theoretical_position, int window_width, int window_height);

    void move(int window_width, int window_height);
    void eat();
    void reproduce();
    void die();
};