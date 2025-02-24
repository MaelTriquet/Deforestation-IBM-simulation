#include "simulation.hpp"
#include <iostream>

int Simulation::id = 0;

Simulation::Simulation(int window_width_, int window_height_, tp::ThreadPool &thread_pool_) : window_width(window_width_),
                                                                                              window_height(window_height_),
                                                                                              grid(window_width, window_height, 2 * MAX_TREES_RADIUS),
                                                                                              ray_grid(window_width, window_height, ANIMALS_RADIUS + RAY_LENGTH),
                                                                                              thread_pool{thread_pool_}
{

    for (int i = 0; i < PREY_START; i++)
    {
        Prey *prey = new Prey(sf::Vector2f{(float)Random::randint(0, window_width), (float)Random::randint(0, window_height)}, id++);
        m_pop.push_back(prey);
    }
    for (int i = 0; i < PRED_START; i++)
    {
        Predator *pred = new Predator(sf::Vector2f{(float)Random::randint(0, window_width), (float)Random::randint(0, window_height)}, id++);
        m_pop.push_back(pred);
    }
    for (int i = 0; i < TREE_START; i++)
    {
        m_trees.push_back(new Tree(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, 0.25));
        m_trees[m_trees.size() - 1]->radius = MAX_TREES_RADIUS;
    }

    grid.init_trees(m_trees);
    ray_grid.init_trees(m_trees);
}

// animals created on heap, must take care of them
Simulation::~Simulation()
{
    for (int i = 0; i < m_pop.size(); i++)
        delete m_pop[i];

    for (int i = 0; i < m_trees.size(); i++)
        delete m_trees[i];
}

void Simulation::update(Actor actorPrey, Critic criticPrey, Actor actorPreda, Critic criticPreda, ReplayBuffer replayBuffer)
{
    // update trees
    if (Random::rand() < PROB_TREE_RANDOM_SPAWN)
        m_trees.push_back(new Tree(sf::Vector2f{(float)Random::randint(window_width), (float)Random::randint(window_height)}, 0.25));
    Tree *new_tree;
    nb_tree = 0;
    for (int i = 0; i < m_trees.size(); i++)
    {
        if (m_trees[i]->is_dead)
            continue;
        nb_tree++;
        new_tree = m_trees[i]->update();
        if (new_tree != 0x0)
        {
            m_trees.push_back(new_tree);
        }
    }

    nb_prey = 0;
    nb_pred = 0;

    // remove dead animals
    for (int i = m_pop.size() - 1; i > -1; i--)
    {
        if (m_pop[i] == 0x0)
            m_pop.erase(m_pop.begin() + i);
    }

    // update animals
    for (int i = 0; i < m_pop.size(); i++)
    {
        m_pop[i]->update();
        if (!m_pop[i]->is_dead)
        {

            torch::Tensor obs = get_observation(m_pop[i]);

            torch::Tensor action;
            thread_pool.addTask([this, i, actorPreda, actorPrey, obs, &action]
                                {
                if (m_pop[i]->is_prey) {
                    action = m_pop[i]->move(WINDOW_WIDTH, WINDOW_HEIGHT, actorPrey, obs);
                } else {
                    action = m_pop[i]->move(WINDOW_WIDTH, WINDOW_HEIGHT, actorPreda, obs);
                } });

                torch::Tensor new_obs = get_observation(m_pop[i]);

            Transition transition{obs, action, 0, new_obs, m_pop[i]->is_dead};
        }
    }
    thread_pool.waitForCompletion();

    // for (int i = m_pop.size() - 1; i > -1; i--) {
    //     m_pop[i]->update();
    //     if (!m_pop[i]->is_dead)
    //         m_pop[i]->move(WINDOW_WIDTH, WINDOW_HEIGHT);
    // }

    for (int i = m_pop.size() - 1; i > -1; i--)
    {
        if (!m_pop[i]->is_dead)
        {
            if (m_pop[i]->is_pred)
                nb_pred++;
            else
                nb_prey++;
            continue;
        }
        if (m_pop[i]->rotting > 0)
            continue;
        m_pop[i]->brain.delete_content();
        delete m_pop[i];
        m_pop.erase(m_pop.begin() + i);
    }

    // std::cout << "Prédateurs : " << nb_pred << ", ";
    // std::cout << "Proies : " << nb_prey << ", ";
    // std::cout << "Morts : " << m_pop.size() - nb_pred - nb_prey << ", ";
    // std::cout << "Arbres : " << nb_tree << ", ";
    // std::cout << "Population : " << m_pop.size() << std::endl;

    for (Animal *a : m_pop)
        a->considerate_bounds(window_width, window_height);
    // update grid cells content
    grid.update_animals(m_pop);
    ray_grid.update_animals(m_pop);
    grid.init_trees(m_trees);
    ray_grid.init_trees(m_trees);

    // check for events
    fill_ray_visions();
    detect_collisions();
}

// handles Animal/Animal collision (fight, eat or reproduce)
void Simulation::collide(Animal *animal_1, Animal *animal_2)
{

    // collision : the animals bounce together
    sf::Vector2f vect_between_centers = (animal_1->position - animal_2->position);
    float norm_vect = std::sqrt(vect_between_centers.x * vect_between_centers.x + vect_between_centers.y * vect_between_centers.y);
    float distance_bounce = (animal_1->radius + animal_2->radius - norm_vect);
    if (distance_bounce > 0 && norm_vect != 0)
    {
        sf::Vector2f vect_normalised = vect_between_centers * (1.f / norm_vect);
        animal_1->position += (distance_bounce + animal_1->radius / 2) * vect_normalised;
        animal_2->position -= (distance_bounce + animal_1->radius / 2) * vect_normalised;
    }

    // animal_1 = prey and animal_2 = predator
    if (animal_1->is_prey && animal_2->is_pred)
        return collide(animal_2, animal_1);

    // animal_1 = predator and animal_2 = prey
    // the predator must see the prey to eat it
    // if the prey can see the predator but the predator can't see the prey, the prey can attack the predator
    if (animal_1->is_pred && animal_2->is_prey && animal_1->has_in_rays(animal_2))
    {
        if (animal_2->is_dead && (animal_1->energy <= MAX_ENERGY || animal_1->health <= MAX_ENERGY))
            return ((Predator *)animal_1)->eat(animal_2);
        ((Predator *)animal_1)->fight(animal_2);
    }
    if (animal_1->is_pred && animal_2->is_prey && !(animal_1->has_in_rays(animal_2)) && animal_2->has_in_rays(animal_1))
    {
        ((Prey *)animal_2)->fight(animal_1);
    }
    if (animal_1->is_pred && animal_2->is_prey)
        return;

    // animal_1 = animal_2 = predator or animal_1 = animal_2 = prey
    if (animal_1->reproduction_timeout <= 0 && animal_2->reproduction_timeout <= 0 && !animal_1->is_dead && !animal_2->is_dead)
    {
        if (animal_1->is_pred && (animal_1->has_eaten || animal_2->has_eaten))
        {
            int nb_child = Random::randint(PRED_N_MIN_CHILDREN, PRED_N_MAX_CHILDREN);
            for (int i = 0; i < nb_child; i++)
            {
                if (nb_pred >= MAX_POP_PRED)
                    break;
                Predator *child = ((Predator *)animal_1)->reproduce((Predator *)animal_2, id++);
                m_pop.push_back(child);
            }
        }
        else if (animal_2->is_prey && (animal_1->has_eaten || animal_2->has_eaten))
        {
            int nb_child = Random::randint(PREY_N_MIN_CHILDREN, PREY_N_MAX_CHILDREN);
            for (int i = 0; i < nb_child; i++)
            {
                if (nb_prey >= MAX_POP_PREY)
                    break;
                Prey *child = ((Prey *)animal_1)->reproduce((Prey *)animal_2, id++);
                m_pop.push_back(child);
            }
        }
    }
}

void Simulation::detect_collisions()
{

    // for (int i = 0; i < grid.width * (grid.height-1); i += 3*grid.width) {
    //     thread_pool.addTask([this, i] {
    //             detect_collisions_threaded(i, i + grid.width);
    //     });
    // }

    // thread_pool.waitForCompletion();

    // for (int i = grid.width; i < grid.width * grid.height; i += 3*grid.width) {
    //     thread_pool.addTask([this, i] {
    //             detect_collisions_threaded(i, i + grid.width);
    //     });
    // }
    // thread_pool.waitForCompletion();

    // for (int i = 2*grid.width; i < grid.width * grid.height; i += 3*grid.width) {
    //     thread_pool.addTask([this, i] {
    //             detect_collisions_threaded(i, i + grid.width);
    //     });
    // }
    // thread_pool.waitForCompletion();

    // if (grid.height % 3 == 1)
    //     detect_collisions_threaded(grid.width * (grid.height-1), grid.width * grid.height);
    detect_collisions_threaded(0, grid.height * grid.width);
}

void Simulation::detect_collisions_threaded(int start, int end)
{

    auto collision_with_animal = [](Animal *animal_1, Animal *animal_2)
    {
        sf::Vector2f temp_vect = (animal_1->position - animal_2->position);
        float squared_distance = temp_vect.x * temp_vect.x + temp_vect.y * temp_vect.y;
        float squared_radius = (animal_1->radius + animal_2->radius) * (animal_1->radius + animal_2->radius);
        return (squared_distance < squared_radius);
    };

    // anonymous function to detect if an animal is colliding with a tree
    auto collision_with_tree = [](Animal *animal, Tree *tree)
    {
        sf::Vector2f temp_vect = (animal->position - tree->position);
        float squared_distance = temp_vect.x * temp_vect.x + temp_vect.y * temp_vect.y;
        float squared_radius = (tree->radius) * (tree->radius);
        return (squared_distance < squared_radius);
    };

    for (int i = start; i < end; i++)
    {
        Cell current_cell = grid.cells[i];
        std::unique_ptr<std::vector<Cell *>> neighbours = grid.get_neighbours(i);

        // iterating on the neighbours of the cell
        for (int j = 0; j < neighbours->size(); j++)
        {
            Cell *neighbour_cell = (*neighbours)[j];

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
            for (Animal *animal_current_cell : current_cell.animals)
            {

                // iterating on the animals of the neighour cell
                for (Animal *animal_neighbour_cell : neighbour_cell->animals)
                {
                    if (animal_current_cell == animal_neighbour_cell)
                        continue;
                    animal_neighbour_cell->position += offset;
                    if (collision_with_animal(animal_current_cell, animal_neighbour_cell))
                    {
                        animal_current_cell->is_colliding = true;
                        animal_neighbour_cell->is_colliding = true;
                        collide(animal_current_cell, animal_neighbour_cell);
                    }
                    animal_neighbour_cell->position -= offset;
                }

                // iterating on the trees of the neighbour cell
                for (Tree *tree : neighbour_cell->trees)
                {
                    if (collision_with_tree(animal_current_cell, tree))
                    {
                        // animal_current_cell->is_colliding = true;
                        collide(tree, animal_current_cell);
                    }
                }
            }
        }
    }
}
void Simulation::fill_ray_visions()
{
    for (int i = 0; i < ray_grid.width * ray_grid.height; i += ray_grid.width)
    {
        thread_pool.addTask([this, i]
                            { fill_ray_visions(i, i + ray_grid.width); });
    }
    thread_pool.waitForCompletion();
}

void Simulation::fill_ray_visions(int start, int end)
{
    for (int v = start; v < end; v++)
    {
        std::unique_ptr<std::vector<Cell *>> neighbours;
        neighbours = ray_grid.get_neighbours(v);
        for (Animal *a : ray_grid.cells[v].animals)
        {
            sf::Vector2f ray;
            for (int i = 0; i < NB_RAY; i++)
            {
                a->vision.rays[i] = 0;
                a->vision.rays[i + NB_RAY] = 0;
                // create the ray
                float theta = -a->max_ray_angle / 2 + i * a->max_ray_angle / (float)(NB_RAY - 1);
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
                for (Cell *neigh : *neighbours)
                {
                    sf::Vector2f offset{0, 0}; // handles detection through tore's bounds
                    if (i % ray_grid.width == 0 && neigh->index % ray_grid.width == ray_grid.width - 1)
                        offset.x = -window_width;
                    if (neigh->index % ray_grid.width == 0 && i % ray_grid.width == ray_grid.width - 1)
                        offset.x = window_width;

                    if (i / ray_grid.width == 0 && neigh->index / ray_grid.width == ray_grid.height - 1)
                        offset.y = -window_width;
                    if (neigh->index / ray_grid.width == 0 && i / ray_grid.width == ray_grid.height - 1)
                        offset.y = window_width;

                    for (Animal *a2 : neigh->animals)
                    {
                        if (a->index == a2->index)
                            continue;
                        float dist = std::sqrt((a->position.x - a2->position.x - offset.x) * (a->position.x - a2->position.x - offset.x) + (a->position.y - a2->position.y - offset.y) * (a->position.y - a2->position.y - offset.y));
                        if (dist > RAY_LENGTH + ANIMALS_RADIUS)
                            continue;
                        float res = segmentIntersectsCircle(a->position, ray, a2->position + offset, ANIMALS_RADIUS);
                        if (res < 0)
                            continue;

                        if (a->vision.rays[i] < res)
                        {
                            a->vision.rays[i] = res;
                            if (a->is_pred)
                            {
                                a->vision.rays[i + NB_RAY] = 1;
                            }
                            else
                            {
                                if (a2->is_prey && !a2->is_dead)
                                    a->vision.rays[i + NB_RAY] = 1;
                                else if (a2->is_pred)
                                    a->vision.rays[i + NB_RAY] = -1;
                                else
                                    a->vision.rays[i + NB_RAY] = 0;
                            }
                        }
                    }

                    for (Tree *t : neigh->trees)
                    {
                        float dist = std::sqrt((a->position.x - t->position.x - offset.x) * (a->position.x - t->position.x - offset.x) + (a->position.y - t->position.y - offset.y) * (a->position.y - t->position.y - offset.y));
                        if (dist > RAY_LENGTH + MAX_TREES_RADIUS)
                            continue;
                        float res = segmentIntersectsCircle(a->position, ray, t->position + offset, MAX_TREES_RADIUS);
                        if (res < 0)
                            continue;
                        if (a->vision.rays[i] < res)
                        {
                            a->vision.rays[i] = res;
                            if (a->is_pred)
                            {
                                a->vision.rays[i + NB_RAY] = 0;
                            }
                            else
                            {
                                a->vision.rays[i + NB_RAY] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

float Simulation::segmentIntersectsCircle(const sf::Vector2f &A, const sf::Vector2f &AB, const sf::Vector2f &C, int radius)
{
    // Helper function to calculate the distance between two points
    auto distance = [](const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    };

    if (distance(A, C) < radius)
        return 1.0f;

    // Coefficients for the quadratic equation
    float a = AB.x * AB.x + AB.y * AB.y;
    float b = 2 * (AB.x * (A.x - C.x) + AB.y * (A.y - C.y));
    float c = (A.x - C.x) * (A.x - C.x) + (A.y - C.y) * (A.y - C.y) - radius * radius;

    // Discriminant of the quadratic equation
    float discriminant = b * b - 4 * a * c;

    // If the discriminant is negative, there are no real roots (no intersection)
    if (discriminant < 0)
    {
        return -1.0f;
    }

    // Calculate the roots of the quadratic equation
    float t1 = (-b + std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b - std::sqrt(discriminant)) / (2 * a);

    // Check if the roots are within the range [0, 1]
    if (t1 >= 0 && t1 <= 1)
    {
        sf::Vector2f intersectionPoint1 = A + t1 * AB;
        return 1 - t1;
    }

    if (t2 >= 0 && t2 <= 1)
    {
        sf::Vector2f intersectionPoint2 = A + t2 * AB;
        return 1 - t2;
    }

    // No intersection within the segment
    return -1.0f;
}

// check invisibility
void Simulation::collide(Tree *t, Animal *a)
{
    a->is_in_tree = true;
    if (!a->in_tree)
        a->in_tree = t;
    if (a->is_prey)
    {
        if ((a->energy <= MAX_ENERGY || a->health <= MAX_ENERGY) && !a->is_dead)
            ((Prey *)a)->eat();
    }
    if (a->in_tree == t)
        return;
    a->in_tree = t;
    if (Random::rand() < t->hiding_prob)
    {
        a->invisible = INVISIBILITY_TIME;
    }
}

// Get animal observation vector as torch::Tensor
torch::Tensor Simulation::get_observation(Animal *a)
{
    std::vector<float> obs;
    obs.push_back(a->vision.energy);
    obs.push_back(a->vision.health);
    obs.push_back(a->vision.velocity.x);
    obs.push_back(a->vision.velocity.y);
    for (int i = 0; i < 2 * NB_RAY; i++)
    {
        obs.push_back(a->vision.rays[i]);
    }
    return torch::tensor(obs);
}
