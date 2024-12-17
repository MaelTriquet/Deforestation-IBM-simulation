#include "grid.hpp"
#include <array>

Grid::Grid(int win_width, int win_height, float radius_)
    : radius{radius_}
{
    width = (win_width - 1) / radius;
    height = (win_height - 1) / radius;
    cells = new Cell[width * height];
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            cells[i * width + j] = Cell(i * width + j);
}

Grid::~Grid() {
    delete[] cells;
}

void Grid::update_content(vector<Animal>& pop, vector<Tree>& trees) const {
    for (Cell c : cells) {
        c.animals.clear();
        c.trees.clear();
    }

    for (Animal& a : pop) {
        int i = 
    }
}