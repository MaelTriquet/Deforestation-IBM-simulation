#pragma once
#include <vector>

class Simulation {
private:
    std::vector<Animal> m_pop;
public:
    Simulation();

    void update() {}
};