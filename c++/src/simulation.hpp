#pragma once
#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include "predator.hpp"
#include "prey.hpp"
#include "grid.hpp"

class Simulation {
private:
public:
    static int id; // next animal id
    std::vector<Animal*> m_pop; // alive pop
    std::vector<Tree*> m_trees; // tree pop
    int window_width;
    int window_height;
    // collision grid
    Grid grid;
    //ray detection grid (different size)
    Grid ray_grid;
    bool is_prey_dominating = true;

    int nb_prey = 1;
    int nb_pred = 1;
    int nb_tree = 1;

    Simulation(int window_width_, int window_height_);
    ~Simulation();

    void update();
    void fill_ray_visions();
    void detect_collisions();
    float segmentIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& AB, const sf::Vector2f& C, int radius);
    void collide(Tree* t, Animal* a);
    void collide(Animal* animal_1, Animal* animal_2);
};