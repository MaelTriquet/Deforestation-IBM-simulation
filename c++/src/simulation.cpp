#include "simulation.hpp"
#include "random.hpp"

Simulation::Simulation(int window_width_, int window_height_) :
    window_width(window_width_),
    window_height(window_height_)
{
    for (int i = 0; i < 100; i++) {
        Prey* prey = new Prey(1, sf::Vector2f{Random::randint(window_width), Random::randint(window_height)}, sf::Vector2f{1, 1});
        m_pop.push_back(prey);
    }
    for (int i = 0; i < 100; i++) {
        Predator* pred = new Predator(1, sf::Vector2f{Random::randint(window_width), Random::randint(window_height)}, sf::Vector2f{-1, 1});
        m_pop.push_back(pred);
    }
    for (int i = 0; i < 100; i++) {
        Tree* tree = new Tree(sf::Vector2f{Random::randint(window_width), Random::randint(window_height)}, 0.25);
        m_trees.push_back(tree);
    }
}

Simulation::~Simulation() {
    for (int i = 0; i < m_pop.size(); i++)
        delete m_pop[i];
    for (int i = 0; i < m_trees.size(); i++)
        delete m_trees[i];
}

void Simulation::update() {
    for (Animal* a : m_pop)
        a->move(window_width, window_height);
}
