#pragma once
#include <SFML/Graphics.hpp>

#define RADIUS 10

class Tree {
public:
    int radius = RADIUS;
    sf::Color color{0, 255, 0};
    float hiding_prob;
    sf::Vector2f position;

    Tree(sf::Vector2f position_, float hiding_prob_);
};
