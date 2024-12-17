#include "simulation.hpp"
#include "random.hpp"

Simulation::Simulation() {
    for (int i = 0; i < 100; i++)
    m_pop.emplace_back(1, sf::Vector2f{Random::randint(800), Random::randint(800)}, sf::Vector2f{1, 1});
}

void Simulation::update() {
    for (Animal& a : m_pop)
        a.move();
}
