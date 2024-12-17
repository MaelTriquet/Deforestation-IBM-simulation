#include "simulation.hpp"

Simulation::Simulation() {
    for (int i = 0; i < 100; i++)
    m_pop.emplace_back(1, sf::Vector2f{400, 400}, sf::Vector2f{1, 1}, 20);
}

void Simulation::update() {
    for (Animal& a : m_pop)
        a.move();
}
