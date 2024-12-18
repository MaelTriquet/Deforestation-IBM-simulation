#include "simulation.hpp"
#include "random.hpp"
#include <iostream>

int Simulation::id = 0;

Simulation::Simulation(int window_width_, int window_height_) :
    window_width(window_width_),
    window_height(window_height_),
    grid{window_width, window_height, RADIUS}
{
    for (int i = 0; i < 100; i++) {
        Prey* prey = new Prey(1, sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, sf::Vector2f{-1, -1}, id++);
        m_pop.push_back(prey);
    }
    for (int i = 0; i < 100; i++) {
        Predator* pred = new Predator(1, sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, sf::Vector2f{-1, 1}, id++);
        m_pop.push_back(pred);
    }
    for (int i = 0; i < 150; i++) {
        m_trees.emplace_back(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, 0.25);
    }

    grid.init_trees(m_trees);
}

Simulation::~Simulation() {
    for (int i = 0; i < m_pop.size(); i++)
        delete m_pop[i];
}

void Simulation::update() {
    for (int i = m_pop.size() - 1; i > -1; i--) {
        if (!m_pop[i]->is_dead) {
            m_pop[i]->move(window_width, window_height);
            m_pop[i]->is_colliding = false;
            continue;
        }
        if (m_pop[i]->rotting-- > 0) continue;

        delete m_pop[i];
        m_pop.erase(m_pop.begin() + i);
    }

    grid.update_animals(m_pop);
    detect_collisions();
    fill_ray_visions();
}

void Simulation::detect_collisions() {

    /*
    objectifs :
    - prendre en compte le tore
    */

    // anonymous function to detect if two given animals / trees are colliding
    auto is_colliding_with_animal = [](Animal* animal_1, Animal* animal_2) {
        sf::Vector2f temp_vect = (animal_1->position - animal_2->position);
        float squared_distance = temp_vect.x*temp_vect.x + temp_vect.y*temp_vect.y;
        float squared_radius = (animal_1->radius + animal_2->radius)*(animal_1->radius + animal_2->radius);
        return (squared_distance < squared_radius);
    };

    // anonymous function to detect if two given animals / trees are colliding
    auto is_colliding_with_tree = [](Animal* animal, Tree* tree) {
        sf::Vector2f temp_vect = (animal->position - tree->position);
        float squared_distance = temp_vect.x*temp_vect.x + temp_vect.y*temp_vect.y;
        float squared_radius = (animal->radius + tree->radius)*(animal->radius + tree->radius);
        return (squared_distance < squared_radius);
    };

    // iterating on the cells
    for (int i = 0; i < grid.width * grid.height; i++) {
        Cell actual_cell = grid.cells[i];
        std::unique_ptr<std::vector<Cell*>> neighbours = grid.get_neighbours(i);

        // iterating on the neighbours of the cell
        for (int j = 0; j < 9; j++) {
            Cell* neighbour_cell = (*neighbours)[j];

            // iterating on the animals of the cell
            for (int k = 0; k < actual_cell.animals.size(); k++) {

                // iterating on the animals of the neighour cell
                for (int l = 0; l < neighbour_cell->animals.size(); l++) {
                    if (actual_cell.animals[k] == neighbour_cell->animals[l]) {
                        continue;
                    }
                    if (is_colliding_with_animal(actual_cell.animals[k], neighbour_cell->animals[l])) {
                        actual_cell.animals[k]->is_colliding = true;
                        neighbour_cell->animals[l]->is_colliding = true;
                    }
                }

                // iterating on the trees of the neighbour cell
                for (int l = 0; l < neighbour_cell->trees.size(); l++) {
                    if (is_colliding_with_tree(actual_cell.animals[k], neighbour_cell->trees[l])) {
                        actual_cell.animals[k]->is_colliding = true;
                    }
                }
            }
        }
    }
}

void Simulation::fill_ray_visions() {
    std::unique_ptr<std::vector<Cell*>> neighbours;
    for (int i = 0; i < grid.width * grid.height; i++) {
        neighbours = grid.get_neighbours(i);
        for (Animal* a : grid.cells[i].animals) {
            sf::Vector2f ray;
            for (int i = 0; i < NB_RAY; i++) {
                float theta = -a->max_ray_angle/2 + i * a->max_ray_angle / (float)(NB_RAY-1);
                float alpha;
                if (a->velocity.x * a->velocity.y == 0 && a->velocity.x + a->velocity.y == 0)
                    alpha = 0;
                else if (a->velocity.x == 0)
                    alpha = a->velocity.y < 0 ? M_PI_2 : -M_PI_2;
                else if (a->velocity.x > 0)
                    alpha = std::atan(-a->velocity.y / a->velocity.x);
                else
                    alpha = M_PI + std::atan(-a->velocity.y / a->velocity.x);
                ray = sf::Vector2f(std::cos(alpha + theta), -std::sin(alpha + theta));
                ray *= (float)RAY_LENGTH;
                for (Cell* neigh : *neighbours) {
                    sf::Vector2f offset{0, 0}; // handles detection through tore's bounds
                    if (i % grid.width == 0 && neigh->index % grid.width == grid.width - 1)
                        offset.x = -window_width;
                    if (neigh->index % grid.width == 0 && i % grid.width == grid.width - 1)
                        offset.x = window_width;
                    
                    if (i / grid.width == 0 && neigh->index / grid.width == grid.width - 1)
                        offset.y = -window_width;
                    if (neigh->index / grid.width == 0 && i / grid.width == grid.width - 1)
                        offset.y = window_width;
                    
                    for (Animal* a2 : neigh->animals) {
                        if (a-> index == a2->index) continue;
                        a2->position += offset;
                        float dist = std::sqrt((a->position.x - a2->position.x) * (a->position.x - a2->position.x) + (a->position.y - a2->position.y) * (a->position.y - a2->position.y));
                        a2->position -= offset;
                        if (dist > RAY_LENGTH + RADIUS) continue;
                        float res = segmentIntersectsCircle(a->position, ray, a2->position + offset, RADIUS);
                        if (res < 0) continue;

                        if (a2->is_pred)
                            a->vision.rays[i] = res;
                        else 
                            a->vision.rays[i + NB_RAY] = res;
                    }

                    for (Tree* t : neigh->trees) {
                        t->position += offset;
                        float dist = std::sqrt((a->position.x - t->position.x) * (a->position.x - t->position.x) + (a->position.y - t->position.y) * (a->position.y - t->position.y));
                        t->position -= offset;
                        if (dist > RAY_LENGTH + RADIUS) continue;
                        float res = segmentIntersectsCircle(a->position, ray, t->position + offset, RADIUS);
                        if (res < 0) continue; 
                        a->vision.rays[i + 2*NB_RAY] = res;
                    }
                }
            }
        }
    }
}

float Simulation::segmentIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& AB, const sf::Vector2f& C, int radius) {
    // Helper function to calculate the distance between two points
    auto distance = [](const sf::Vector2f& p1, const sf::Vector2f& p2) {
        return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    };

    // Coefficients for the quadratic equation
    float a = AB.x * AB.x + AB.y * AB.y;
    float b = 2 * (AB.x * (A.x - C.x) + AB.y * (A.y - C.y));
    float c = (A.x - C.x) * (A.x - C.x) + (A.y - C.y) * (A.y - C.y) - radius * radius;

    // Discriminant of the quadratic equation
    float discriminant = b * b - 4 * a * c;

    // If the discriminant is negative, there are no real roots (no intersection)
    if (discriminant < 0) {
        return -1.0f;
    }

    // Calculate the roots of the quadratic equation
    float t1 = (-b + std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b - std::sqrt(discriminant)) / (2 * a);

    // Check if the roots are within the range [0, 1]
    if (t1 >= 0 && t1 <= 1) {
        sf::Vector2f intersectionPoint1 = A + t1 * AB;
        return distance(A + AB, intersectionPoint1);
    }

    if (t2 >= 0 && t2 <= 1) {
        sf::Vector2f intersectionPoint2 = A + t2 * AB;
        return distance(A + AB, intersectionPoint2);
    }

    // No intersection within the segment
    return -1.0f;
}
