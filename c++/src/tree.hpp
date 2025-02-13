#pragma once
#include <SFML/Graphics.hpp>
#include "const.hpp"

class Tree {
public:
    int radius = TREES_RADIUS;
    sf::Color color{0, 255, 0};
    float hiding_prob;
    sf::Vector2f position;
    int nb_fruit = 0;
    int time_before_fruit = FRUIT_TIME;

    Tree(sf::Vector2f position_, float hiding_prob_);

    void update();
};
