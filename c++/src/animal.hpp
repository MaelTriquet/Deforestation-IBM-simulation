#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>
#include <iostream>
#include "tree.hpp"
#include "vision.hpp"
#include "brain.hpp"
#include "random.hpp"

#define RADIUS 10
#define NB_RAY 5
#define RAY_LENGTH 50
#define ROT_TIME 30
#define INVISIBILITY_TIME 30
#define PRED_EATING_ENERGY 10
#define PREY_LOST_ENERGY_FIGHT 2
#define PRED_LOST_ENERGY_FIGHT 2
#define INITIAL_ENERGY 100
#define LOST_ENERGY_PER_REPRODUCTION 1
#define REPRODUCTION_TIMEOUT 200

class Animal {
public:
    int radius = RADIUS;
    int energy = INITIAL_ENERGY;
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
    int reproduction_timeout = 0;

    Animal(sf::Vector2f position_, sf::Vector2f velocity_, int index_);

    void considerate_bounds(int window_width, int window_height);

    void move(int window_width, int window_height);
    void die();
    void look();
    void update();
};
