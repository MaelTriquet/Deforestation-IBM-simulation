#include <vector>
#include "animal.hpp"
#include "tree.hpp"
#include <memory>

struct Cell
{
    std::vector<Animal> animals;
    std::vector<Tree> trees;
    int index;

    Cell(int index_)
        : index{index_}
    {}
};


class Grid {
    Cell* cells;
    int radius;
    int width;
    int height;

    Grid(int win_width, int win_height, int radius_);
    ~Grid();

    void update_content();

    std::unique_ptr<std::vector<Cell>> get_neighbours(int index);
};