#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include <memory>
#include <array>
#include <cmath>

struct Cell
{
    std::vector<Animal*> animals{20};
    std::vector<Tree*> trees;
    int index;

    Cell(int index_)
        : index{index_}
    {}

    Cell() = default;
};

// Used for optimisation purposes, makes collision detection from O(n²) to O(m²) where m is the number of neighbouring agents
class Grid {
public:
    Cell* cells; // array
    int radius; 
    int width;
    int height;

    // creates a grid with the right dimensions
    // radius must be set to the max distance between 2 objects for a collision to happen
    Grid(int win_width, int win_height, int radius_);
    ~Grid();

    void update_animals(std::vector<Animal*>& pop) const;
    void init_trees(std::vector<Tree*>& trees) const;

    std::unique_ptr<std::vector<Cell*>> get_neighbours(int index);
};