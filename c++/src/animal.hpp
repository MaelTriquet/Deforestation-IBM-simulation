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
    int radius = RADIUS; // size of the animal
    int energy; // health
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool is_dead;
    sf::Color color;
    int fleeing = 0; // fright-meter, set high when spotting an ennemy
    Vision vision; // brain input
    float max_ray_angle; // side vision
    int index; // to compare 2 animals
    bool is_pred;
    bool is_prey;
    bool is_colliding = false;
    bool is_in_tree = false;
    const Tree* in_tree = nullptr; // if is_in_tree = true, this is the pointer to that tree
    int rotting = ROT_TIME; // how long a corpse stays around after death
    int invisible = 0; // camouflage counter when hiding in a tree successfully
    float max_velocity;
    float max_vel_percent = .6; // will dissapear
    Brain brain; // decision maker
    float decision[3] = {0}; // holds the decision after the thinking process


    Animal(int energy_, sf::Vector2f position_, sf::Vector2f velocity_, int index_);

    void considerate_bounds(int window_width, int window_height);

    void move(int window_width, int window_height);
    void reproduce(Animal* animal);
    void die();
    void look();
    void update();
};
