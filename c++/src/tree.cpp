#include "tree.hpp"

Tree::Tree(sf::Vector2f position_, float hiding_prob_) :
    position(position_),
    hiding_prob(hiding_prob_)
{};

void Tree::update() {
    time_before_fruit--;
    if (time_before_fruit > 0) return;
    time_before_fruit = FRUIT_TIME;
    nb_fruit++;
}