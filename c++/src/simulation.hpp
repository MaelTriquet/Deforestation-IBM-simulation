#pragma once
#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include "predator.hpp"
#include "prey.hpp"
#include "grid.hpp"

class Simulation {
private:
public:
    static int id;
    std::vector<Animal*> m_pop;
    std::vector<Tree> m_trees;
    int window_width;
    int window_height;
    Grid grid;
    Grid ray_grid;

    Simulation(int window_width_, int window_height_);
    ~Simulation();

    void update();
    void fill_ray_visions();
    void detect_collisions();
    float segmentIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& AB, const sf::Vector2f& C, int radius);
};