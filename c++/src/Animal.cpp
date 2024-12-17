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
        
        Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_) : 
            energy(energy_),
            position(position_),
            velocity(velocity_),
            is_dead(false)
        {};

        void move() {
            position += velocity;
        }

        void eat() {
        }

        void reproduce() {
        }

        void die() {
            is_dead = (energy <= 0);
        }
        
};