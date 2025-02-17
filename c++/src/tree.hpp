#pragma once
#include <SFML/Graphics.hpp>
#include "const.hpp"
#include "random.hpp"
class Tree {
public:
    float radius = Random::randint(3, MAX_TREES_RADIUS);
    sf::Color color{0, 255, 0};
    float hiding_prob;
    sf::Vector2f position;
    int max_radius = MAX_TREES_RADIUS;
    bool has_reproduced = false;
    bool is_dead = false;

    Tree(sf::Vector2f position_, float hiding_prob_);

    Tree* update();
};
