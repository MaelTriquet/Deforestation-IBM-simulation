#include "simulation.hpp"
#include <iostream>

int Simulation::id = 0;

Simulation::Simulation(int window_width_, int window_height_) :
    window_width(window_width_),
    window_height(window_height_),
    grid(window_width, window_height, 2*ANIMALS_RADIUS),
    ray_grid(window_width, window_height, ANIMALS_RADIUS + RAY_LENGTH)
{

    for (int i = 0; i < PREY_START; i++) {
        Prey* prey = new Prey(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, id++);
        m_pop.push_back(prey);
    }
    for (int i = 0; i < PRED_START; i++) {
        Predator* pred = new Predator(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, id++);
        m_pop.push_back(pred);
    }
    for (int i = 0; i < TREE_START; i++) {
        m_trees.emplace_back(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, 0.25);
    }

    grid.init_trees(m_trees);
    ray_grid.init_trees(m_trees);
}

// animals created on heap, must take care of them
Simulation::~Simulation() {
    for (int i = 0; i < m_pop.size(); i++)
        delete m_pop[i];
}


void Simulation::update() {
    for (Tree& t : m_trees)
        t.update();
    float nb_prey = 0.f;
    float nb_pred = 0.f;
    for (int i = m_pop.size() - 1; i > -1; i--) {
        m_pop[i]->update();
        if (!m_pop[i]->is_dead) {
            m_pop[i]->move(window_width, window_height);
            if (m_pop[i]->is_pred)
                nb_pred++;
            else 
                nb_prey++;
            continue;
        }
        if (m_pop[i]->rotting > 0) continue;
        m_pop[i]->brain.delete_content();
        delete m_pop[i];
        m_pop.erase(m_pop.begin() + i);
    }

    is_prey_dominating = nb_pred < nb_prey;

    // if (nb_prey == 0)
    // for (int i = 0; i < 10; i++) {
    //     Prey* prey = new Prey(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, id++);
    //     m_pop.push_back(prey);
    // }
    // if (nb_pred == 0)
    // for (int i = 0; i < 10; i++) {
    //     Predator* pred = new Predator(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, id++);
    //     m_pop.push_back(pred);
    // }

    std::cout << "Prédateurs : " << nb_pred / m_pop.size() * 100 << "%, ";
    std::cout << "Proies : " << nb_prey / m_pop.size() * 100 << "%, " << "Population : " << m_pop.size() << std::endl;

    // update grid cells content
    grid.update_animals(m_pop);
    ray_grid.update_animals(m_pop);

    // check for events
    detect_collisions();
    fill_ray_visions();
}

// handles Animal/Animal collision (fight, eat or reproduce)
void Simulation::collide(Animal* animal_1, Animal* animal_2) {

    // animal_1 = prey and animal_2 = predator
    if (animal_1->is_prey && animal_2->is_pred)
        return collide(animal_2, animal_1);

    // animal_1 = predator and animal_2 = prey
    if (animal_1->is_pred && animal_2->is_prey) {
        if (!animal_1->is_dead && animal_2->is_dead && animal_1->energy <= MAX_ENERGY)
            return ((Predator*)animal_1)->eat(animal_2);
        return ((Predator*)animal_1)->fight(animal_2);
    }

    // animal_1 = animal_2 = predator or animal_1 = animal_2 = prey
    if (animal_1->reproduction_timeout <= 0 && animal_2->reproduction_timeout <= 0 && !animal_1->is_dead && !animal_2->is_dead) {
        if (animal_1->is_pred && (m_pop.size() < MAX_POP || (is_prey_dominating && m_pop.size() < MAX_POP * 1.5))) {
            int nb_child = Random::randint(1, 4);
            for (int i = 0; i < nb_child; i++) {
                Predator* child = ((Predator*)animal_1)->reproduce((Predator*)animal_2, id++);
                m_pop.push_back(child);
            }
        } else if (animal_2->is_prey && (m_pop.size() < MAX_POP || (!is_prey_dominating && m_pop.size() < MAX_POP * 1.5))) {
            int nb_child = Random::randint(2, 7);
            for (int i = 0; i < nb_child; i++) {
                Prey* child = ((Prey*)animal_1)->reproduce((Prey*)animal_2, id++);
                m_pop.push_back(child);
            }
        }
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
        float squared_radius = (tree->radius)*(tree->radius);
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
                        // animal_current_cell->is_colliding = true;
                        collide(*tree, animal_current_cell);
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
                // create the ray
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

                // check if anything intersects the ray
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
                        if (dist > RAY_LENGTH + ANIMALS_RADIUS) continue;
                        float res = segmentIntersectsCircle(a->position, ray, a2->position + offset, ANIMALS_RADIUS);
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
                        if (dist > RAY_LENGTH + TREES_RADIUS) continue;
                        float res = segmentIntersectsCircle(a->position, ray, t->position + offset, TREES_RADIUS);
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

// check invisibility
void Simulation::collide(Tree& t, Animal* a) {
    a->is_in_tree = true;
    if (a->is_prey) {
        if (a->in_tree == &t && a->energy <= MAX_ENERGY) return ((Prey*)a)->eat();
        a->in_tree = &t;
        if (a->energy <= MAX_ENERGY) ((Prey*)a)->eat();
    }
    if (a->in_tree == &t) return;
    a->in_tree = &t;
    if (Random::rand() < t.hiding_prob)
        a->invisible = INVISIBILITY_TIME;
}