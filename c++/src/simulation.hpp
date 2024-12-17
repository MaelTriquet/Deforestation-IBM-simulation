#pragma once
#include <vector>
#include "animal.hpp"
#include "tree.hpp"

class Simulation {
private:
public:
    std::vector<Animal> m_pop;
    Simulation();

    void update();
};