#include "grid.hpp"
#include "iostream"

Grid::Grid(int win_width, int win_height, int radius_)
    : radius{radius_}
{
    width = (win_width - 1) / radius + 1;
    height = (win_height - 1) / radius + 1;
    cells = new Cell[width * height];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            cells[i * width + j] = Cell(i * width + j);
}

Grid::~Grid() {
    delete[] cells;
}

void Grid::update_animals(std::vector<Animal*>& pop) const {
    for (int i = 0; i < width * height; i++)
        cells[i].animals.clear();
    int i, j;
    for (Animal* a : pop) {
        j = std::floor(a->position.x) / radius;
        i = std::floor(a->position.y) / radius;
        cells[i * width + j].animals.push_back(a);
    }
}

void Grid::init_trees(std::vector<Tree>& trees) const {
    int i, j;
    for (Tree& t : trees) {
        j = std::floor(t.position.x) / radius;
        i = std::floor(t.position.y) / radius;
        cells[i * width + j].trees.push_back(&t);
    }
}

std::unique_ptr<std::vector<Cell*>> Grid::get_neighbours(int index) {
    std::unique_ptr<std::vector<Cell*>> res = std::make_unique<std::vector<Cell*>>();
    res->push_back(cells + index);
    int i = index / width;
    int j = index % width;
    res->push_back(cells + ((height + i - 1) % height) * width + (width + j - 1) % width);
    res->push_back(cells + ((height + i - 1) % height) * width + j);
    res->push_back(cells + ((height + i - 1) % height) * width + (j + 1) % width);
    res->push_back(cells + (i) * width + (width + j - 1) % width);
    res->push_back(cells + (i) * width + (j + 1) % width);
    res->push_back(cells + ((i + 1) % height) * width + (width + j - 1) % width);
    res->push_back(cells + ((i + 1) % height) * width + j);
    res->push_back(cells + ((i + 1) % height) * width + (j + 1) % width);

    return res;
}