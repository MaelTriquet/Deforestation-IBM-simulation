#pragma once
#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include "predator.hpp"
#include "prey.hpp"
#include "grid.hpp"
#include "thread_pool.hpp"
#include "actor.hpp"
#include "critic.hpp"
#include "replayBuffer.hpp"

class Simulation {
private:
public:
    static int id; // next animal id
    std::vector<Animal*> m_pop{2 * (MAX_POP_PRED + MAX_POP_PREY)}; // alive pop
    std::vector<Tree*> m_trees; // tree pop
    int window_width;
    int window_height;
    // collision grid
    Grid grid;
    //ray detection grid (different size)
    Grid ray_grid;

    tp::ThreadPool& thread_pool;

    int nb_prey = 2;
    int nb_pred = 2;
    int nb_tree = 2;

    Simulation(int window_width_, int window_height_, tp::ThreadPool& thread_pool_);
    ~Simulation();

    void update(Actor actorPrey, Critic criticPrey, Actor actorPreda, Critic criticPreda, ReplayBuffer replayBuffer);
    void fill_ray_visions();
    void fill_ray_visions(int start, int end);
    void detect_collisions();
    float segmentIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& AB, const sf::Vector2f& C, int radius);
    void collide(Tree* t, Animal* a);
    void collide(Animal* animal_1, Animal* animal_2);
    void detect_collisions_threaded(int start, int end);
    torch::Tensor get_observation(Animal* a);


};