#pragma once
#include <vector>
#include "animal.hpp"

class Simulation {
private:
public:
    std::vector<Animal> m_pop;
    Simulation();

    void update();
};