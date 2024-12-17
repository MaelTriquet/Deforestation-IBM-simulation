#pragma once
#include <vector>
#include "Animal.cpp"

class Simulation {
private:
public:
    std::vector<Animal> m_pop;
    Simulation();

    void update();
};