#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>
#include <iostream>
#include "tree.hpp"
#include "vision.hpp"
#include "brain.hpp"
#include "random.hpp"
#include "const.hpp"

class Animal {
public:
    int index; // to compare 2 animals
    int radius = ANIMALS_RADIUS;
    float energy = INITIAL_ENERGY;
    float health = MAX_HEALTH;
    sf::Vector2f position;
    sf::Vector2f velocity{0, 0};
    float max_velocity;
    int fleeing = 0; // fright-meter, set high when spotting an ennemy
    Vision vision; // brain input
    float max_ray_angle; // side vision
    Brain brain; // decision maker
    float decision[2] = {0}; // holds the decision after the thinking process
    bool is_pred;
    bool is_prey;
    bool is_dead;
    bool has_eaten = false;
    bool is_colliding = false;
    bool is_in_tree = false;
    Tree* in_tree = nullptr; // if is_in_tree = true, this is the pointer to that tree
    int rotting = ROT_TIME; // how long a corpse stays around after death
    int invisible = 0; // camouflage counter when hiding in a tree successfully
    int reproduction_timeout = REPRODUCTION_TIMEOUT;
    sf::Color color;

    Animal(sf::Vector2f position_, int index_);

    void considerate_bounds(int window_width, int window_height);

    void move(int window_width, int window_height);
    void die();
    void look();
    void update();
    bool has_in_rays(Animal* animal);
};
