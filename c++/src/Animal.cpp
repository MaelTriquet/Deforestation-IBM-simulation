#pragma once
#include "animal.hpp"

Animal::Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_) : 
    energy(energy_),
    position(position_),
    velocity(velocity_),
    is_dead(false)
{};

void Animal::move() {
    position += velocity;
};

void Animal::eat() {

};

void Animal::reproduce() {

};

void Animal::die() {
    is_dead = (energy <= 0);
};
