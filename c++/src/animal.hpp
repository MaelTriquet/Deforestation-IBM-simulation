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

        void move();
        void eat();
        void reproduce();
        void die();
};