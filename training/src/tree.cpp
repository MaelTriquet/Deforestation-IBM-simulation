#include "tree.hpp"

Tree::Tree(sf::Vector2f position_, float hiding_prob_) :
    position(position_),
    hiding_prob(hiding_prob_)
{};

Tree* Tree::update() {
    if (radius < MIN_TREES_RADIUS)
        is_dead = true;
    if (is_dead) return 0x0;
    Tree* new_tree = 0x0;
    if (radius < max_radius) {
        if (radius < (float)max_radius * TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT)
            has_reproduced = false;
        radius += (float)MAX_TREES_RADIUS / TIME_TREE_GROWTH;
    } else if (!has_reproduced) {
        float x = (Random::rand()*2-1) * max_radius * 3;
        float y = (Random::rand()*2-1) * max_radius * 3;
        if (position.x + x < 0)
            x += WINDOW_WIDTH;
        if (position.x + x >= WINDOW_WIDTH)
            x -= WINDOW_WIDTH;
        if (position.y + y < 0)
            y += WINDOW_HEIGHT;
        if (position.y + y >= WINDOW_HEIGHT)
            y -= WINDOW_HEIGHT;
        new_tree = new Tree(position + sf::Vector2f(x, y), hiding_prob);
        has_reproduced = true;
    }
    return new_tree;
}