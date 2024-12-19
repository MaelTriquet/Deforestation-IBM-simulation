#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>
#include "brain.hpp"
#include "tree.hpp"
#include "vision.hpp"
#include <iostream>

#define RADIUS 10
#define RAY_LENGTH 50
#define ROT_TIME 30
#define INVISIBILITY_TIME 30
#define PRED_EATING_ENERGY 10
#define PREY_LOST_ENERGY_FIGHT 2

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
    Brain brain;
    float decision[3] = {0};


    Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_);

    void considerate_bounds(int window_width, int window_height);

    void move(int window_width, int window_height);
    void reproduce(Animal* animal);
    void die();
    void look();
    void update();
};
