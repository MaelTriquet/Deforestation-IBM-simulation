#include "simulation.hpp"
#include "random.hpp"

Simulation::Simulation() {
    for (int i = 0; i < 100; i++) {
        Prey* prey = new Prey(1, sf::Vector2f{400, 400}, sf::Vector2f{1, 1});
        m_pop.push_back(prey);
    }
    for (int i = 0; i < 100; i++) {
        Predator* pred = new Predator(1, sf::Vector2f{400, 400}, sf::Vector2f{-1, 1});
        m_pop.push_back(pred);
    }
}

Simulation::~Simulation() {
    for (int i = 0; i < m_pop.size(); i++)
        delete m_pop[i];
}

void Simulation::update() {
    for (Animal* a : m_pop)
        a->move();
}
