#include "simulation.hpp"
#include "random.hpp"
#include <iostream>

Simulation::Simulation(int window_width_, int window_height_) :
    window_width(window_width_),
    window_height(window_height_),
    grid{window_width, window_height, RADIUS}
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

void Simulation::detect_collisions() {
    for (int i = 0; i < grid.width * grid.height; i++) {
        Cell actual_cell = grid.cells[i];
        std::unique_ptr<std::vector<Cell&>> neighbours = grid.get_neighbours(i);
        for (int j = 0; j < 9; j++) {
            Cell neighbour_cell = (*neighbours)[j];
            for (int k = 0; k < actual_cell.animals.size(); k++) {
                for (int l = 0; l < neighbour_cell.trees.size(); l++) {
                    sf::Vector2f temp_vect = (actual_cell.animals[k].position - neighbour_cell.animals[l].position);
                    float squared_distance = pow(temp_vect.x, 2) + pow(temp_vect.y, 2);
                    float squared_radius = pow(actual_cell.animals[k].radius + neighbour_cell.animals[l].radius, 2);
                    if (squared_distance < squared_radius) {
                        actual_cell.animals[k].is_collisioning = true;
                    }
                }
            }
        }
    }
}
