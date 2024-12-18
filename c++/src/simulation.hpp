#pragma once
#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include "predator.hpp"
#include "prey.hpp"

class Simulation {
private:
public:
    std::vector<Animal*> m_pop;
    std::vector<Tree*> m_trees;
    int window_width;
    int window_height;

    Simulation(int window_width_, int window_height_);
    ~Simulation();

    void update();
};