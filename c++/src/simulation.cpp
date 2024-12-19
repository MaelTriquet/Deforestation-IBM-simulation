#include "simulation.hpp"
#include "random.hpp"
#include <iostream>

int Simulation::id = 0;

Simulation::Simulation(int window_width_, int window_height_) :
    window_width(window_width_),
    window_height(window_height_),
    grid(window_width, window_height, 2*RADIUS),
    ray_grid(window_width, window_height, RADIUS + RAY_LENGTH)
{
    for (int i = 0; i < 100; i++) {
        Prey* prey = new Prey(1, sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, sf::Vector2f{0.75, 0.75}, id++);
        m_pop.push_back(prey);
    }
    for (int i = 0; i < 100; i++) {
        Predator* pred = new Predator(1, sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, sf::Vector2f{-0.75, 0.75}, id++);
        m_pop.push_back(pred);
    }
    for (int i = 0; i < 150; i++) {
        m_trees.emplace_back(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, 0.25);
    }

    grid.init_trees(m_trees);
    ray_grid.init_trees(m_trees);
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

void Simulation::collide(Animal* animal_1, Animal* animal_2) {

    // animal_1 = prey, animal_2 = predator
    if (animal_1->is_prey && animal_2->is_pred) {
        std::swap(animal_1, animal_2);
    }

    // animal_1 = predator and animal_2 = prey
    if (animal_1->is_pred && animal_2->is_prey) {
        if (animal_2->is_dead) {
            ((Predator*)animal_1)->eat(animal_2);
        } else {
            ((Predator*)animal_1)->fight(animal_2);
        }
    }

    // animal_1 = animal_2 = predator or animal_1 = animal_2 = prey
    else {
        animal_1->reproduce(animal_2);
    }
}

void Simulation::detect_collisions() {

    // anonymous function to detect if two given animals are colliding
    auto collision_with_animal = [](Animal* animal_1, Animal* animal_2) {
        sf::Vector2f temp_vect = (animal_1->position - animal_2->position);
        float squared_distance = temp_vect.x*temp_vect.x + temp_vect.y*temp_vect.y;
        float squared_radius = (animal_1->radius + animal_2->radius)*(animal_1->radius + animal_2->radius);
        return (squared_distance < squared_radius);
    };

    // anonymous function to detect if an animal is colliding with a tree
    auto collision_with_tree = [](Animal* animal, Tree* tree) {
        sf::Vector2f temp_vect = (animal->position - tree->position);
        float squared_distance = temp_vect.x*temp_vect.x + temp_vect.y*temp_vect.y;
        float squared_radius = (animal->radius + tree->radius)*(animal->radius + tree->radius);
        return (squared_distance < squared_radius);
    };

    // iterating on the cells of the grid
    for (int i = 0; i < grid.width * grid.height; i++) {
        Cell current_cell = grid.cells[i];
        std::unique_ptr<std::vector<Cell*>> neighbours = grid.get_neighbours(i);

        // iterating on the neighbours of the cell
        for (int j = 0; j < neighbours->size(); j++) {
            Cell* neighbour_cell = (*neighbours)[j];

            // take the tore into account (an animal at the top can collide with another one at the bottom)
            sf::Vector2f offset{0, 0};
            if (i % grid.width == 0 && neighbour_cell->index % grid.width == grid.width - 1)
                offset.x = -window_width;
            if (neighbour_cell->index % grid.width == 0 && i % grid.width == grid.width - 1)
                offset.x = window_width;
            if (i / grid.width == 0 && neighbour_cell->index / grid.width == grid.width - 1)
                offset.y = -window_width;
            if (neighbour_cell->index / grid.width == 0 && i / grid.width == grid.width - 1)
                offset.y = window_width;

            // iterating on the animals of the cell
            for (Animal* animal_current_cell : current_cell.animals) {

                // iterating on the animals of the neighour cell
                for (Animal* animal_neighbour_cell : neighbour_cell->animals) {
                    if (animal_current_cell == animal_neighbour_cell) continue;
                    animal_neighbour_cell->position += offset;
                    if (collision_with_animal(animal_current_cell, animal_neighbour_cell)) {
                        animal_current_cell->is_colliding = true;
                        animal_neighbour_cell->is_colliding = true;
                        collide(animal_current_cell, animal_neighbour_cell);
                    }
                    animal_neighbour_cell->position -= offset;
                }

                // iterating on the trees of the neighbour cell
                for (Tree* tree : neighbour_cell->trees) {
                    if (collision_with_tree(animal_current_cell, tree)) {
                        animal_current_cell->is_colliding = true;
                    }
                }
            }
        }
    }
}

void Simulation::fill_ray_visions() {
    std::unique_ptr<std::vector<Cell*>> neighbours;
    for (int i = 0; i < ray_grid.width * ray_grid.height; i++) {
        neighbours = ray_grid.get_neighbours(i);
        for (Animal* a : ray_grid.cells[i].animals) {
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
                    if (i % ray_grid.width == 0 && neigh->index % ray_grid.width == ray_grid.width - 1)
                        offset.x = -window_width;
                    if (neigh->index % ray_grid.width == 0 && i % ray_grid.width == ray_grid.width - 1)
                        offset.x = window_width;
                    
                    if (i / ray_grid.width == 0 && neigh->index / ray_grid.width == ray_grid.height - 1)
                        offset.y = -window_width;
                    if (neigh->index / ray_grid.width == 0 && i / ray_grid.width == ray_grid.height - 1)
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

    if (distance(A, C) < radius) return 1.0f;

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
