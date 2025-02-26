#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <fstream>
#include "simulation.hpp"
#include "thread_pool.hpp"

bool appendCSV(const std::string& filename, Simulation& sim);
bool emptyCSV(const std::string& filename);
void remove_trees(int count, Simulation& simulation);
int main() {

    // Create window
    constexpr int32_t window_width = WINDOW_WIDTH;
    constexpr int32_t window_height = WINDOW_HEIGHT;

    tp::ThreadPool thread_pool(10);

    // sf::ContextSettings settings;
    // settings.antialiasingLevel = 1;
    // sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    // const uint32_t frame_rate = FRAME_RATE;
    // window.setFramerateLimit(frame_rate);

    int MAX_POP_FRAME = 50;
    int max_pop_frame = MAX_POP_FRAME;
    float score = 0.;
    bool up_prey = true;
    bool up_pred = true;
    bool up_tree = true;
    int extremum_prey = 0;
    int extremum_pred = 0;
    int extremum_tree = 0;
    int frame = 0;

    Simulation simulation{window_width, window_height, thread_pool};
    // Renderer renderer{window};

    emptyCSV("../../res/plot_info.csv");

    while (max_pop_frame > 0 && simulation.nb_pred > 1 && simulation.nb_prey > 1) {

        // if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        // {
        //     sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        //     for (int i = simulation.m_trees.size()-1; i > -1; i--)
        //         if (sqrt((simulation.m_trees[i]->position.x - localPosition.x) * (simulation.m_trees[i]->position.x - localPosition.x) + (simulation.m_trees[i]->position.y - localPosition.y) * (simulation.m_trees[i]->position.y - localPosition.y)) < 200) {
        //             delete simulation.m_trees[i];
        //             simulation.m_trees.erase(simulation.m_trees.begin() + i);
        //         }
        //     simulation.grid.init_trees(simulation.m_trees);
        //     simulation.ray_grid.init_trees(simulation.m_trees);

        // }
        
        frame++;
        // remove_trees(frame, simulation);

        // check for user closing the window
        // sf::Event event{};
        // while (window.pollEvent(event)) {
        //     if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        //         window.close();
        //     }
        // }
        
        // update and show each frame
        for (int i = 0; i < 1; i++) {
            simulation.update();
            if (up_prey && simulation.nb_prey > extremum_prey) {
                extremum_prey = simulation.nb_prey;
            }

            if (up_prey && simulation.nb_prey < extremum_prey * .8) {
                up_prey = !up_prey;
                extremum_prey = simulation.nb_prey;
                score++;
            }

            if (!up_prey && simulation.nb_prey < extremum_prey) {
                extremum_prey = simulation.nb_prey;
            }

            if (!up_prey && simulation.nb_prey > extremum_prey * 1.2) {
                up_prey = !up_prey;
                extremum_prey = simulation.nb_prey;
                score++;
            }


            if (up_pred && simulation.nb_pred > extremum_pred) {
                extremum_pred = simulation.nb_pred;
            }

            if (up_pred && simulation.nb_pred < extremum_pred * .8) {
                up_pred = !up_pred;
                extremum_pred = simulation.nb_pred;
                score++;
            }

            if (!up_pred && simulation.nb_pred < extremum_pred) {
                extremum_pred = simulation.nb_pred;
            }

            if (!up_pred && simulation.nb_pred > extremum_pred * 1.2) {
                up_pred = !up_pred;
                extremum_pred = simulation.nb_pred;
                score++;
            }



            if (up_tree && simulation.nb_tree > extremum_tree) {
                extremum_tree = simulation.nb_tree;
            }

            if (up_tree && simulation.nb_tree < extremum_tree * .8) {
                up_tree = !up_tree;
                extremum_tree = simulation.nb_tree;
                score++;
            }

            if (!up_tree && simulation.nb_tree < extremum_tree) {
                extremum_tree = simulation.nb_tree;
            }

            if (!up_tree && simulation.nb_tree > extremum_tree * 1.2) {
                up_tree = !up_tree;
                extremum_tree = simulation.nb_tree;
                score++;
            }
            appendCSV("../../res/plot_info.csv", simulation);
            if (simulation.nb_prey >= 4000)
                max_pop_frame--;
        }
        // window.clear(sf::Color::Black);
        // renderer.render(simulation);
		// window.display();
    }

    std::cout << score << "\n";
    std::cout << Random::seed << std::endl;

    for (int i = 0; i < simulation.m_pop.size(); i++) {
        simulation.m_pop[i]->brain.delete_content();
    }
}

bool emptyCSV(const std::string& filename) {
    // Create an output file stream in truncate mode
    std::ofstream file(filename, std::ios::trunc);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Close the file to ensure changes are saved
    file.close();
    return true;
}


bool appendCSV(const std::string& filename, Simulation& sim) {
    // Create an output file stream in append mode
    std::ofstream file(filename, std::ios::app);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Write data to the file
    file << sim.nb_pred << "," << sim.nb_prey << "," << sim.m_pop.size() - sim.nb_pred - sim.nb_prey << "," << sim.nb_tree << "\n";

    // Close the file
    file.close();
    return true;
}

void remove_trees(int count, Simulation& simulation) {
    if (count % 100 == 0) 
        std::cout << ", Frame : " << count << "\n";
    // if (simulation.nb_tree >= 1500 && simulation.nb_tree <= 1600 && count > 10000) {
    //     std::cout << "1500 arbres : " << count << '\n';
    // }

    // if (simulation.nb_tree >= 2400 && count > 10000) {
    //     std::cout << "2400 arbres : " << count << '\n';
    //     // up = false;
    // }

    int frame_to_remove = 550000;
    if (count == frame_to_remove) {
        bool remove_uniform = false;
        sf::Vector2f center{400, 400};
        int remove_radius = 100;
        int nb_remove = 25;

        if (remove_uniform) {
            for (; nb_remove > 0; nb_remove--) {
                delete simulation.m_trees[nb_remove];
                simulation.m_trees.erase(simulation.m_trees.begin() + nb_remove);
            }
            simulation.grid.init_trees(simulation.m_trees);
            simulation.ray_grid.init_trees(simulation.m_trees);
            return;
        }
        int nb_removed = 0;
        for (int i = simulation.m_trees.size()-1; i > -1; i--)
            if (sqrt((simulation.m_trees[i]->position.x - center.x) * (simulation.m_trees[i]->position.x - center.x) + (simulation.m_trees[i]->position.y - center.y) * (simulation.m_trees[i]->position.y - center.y)) < remove_radius) {
                delete simulation.m_trees[i];
                simulation.m_trees.erase(simulation.m_trees.begin() + i);
                nb_removed++;
            }
        simulation.grid.init_trees(simulation.m_trees);
        simulation.ray_grid.init_trees(simulation.m_trees);
        std::cout << nb_removed << "\n";
    }
}