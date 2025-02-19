#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <fstream>
#include "simulation.hpp"
#include "thread_pool.hpp"

bool appendCSV(const std::string& filename, Simulation& sim);
bool emptyCSV(const std::string& filename);
int main() {

    // Create window
    constexpr int32_t window_width = WINDOW_WIDTH;
    constexpr int32_t window_height = WINDOW_HEIGHT;

    tp::ThreadPool thread_pool(10);

    // sf::ContextSettings settings;
    // settings.antialiasingLevel = 1;
    // sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    // const uint32_t frame_rate = 60;
    // window.setFramerateLimit(frame_rate);

    int max_pop_frame = 1500;
    float score = 0.;
    bool pred_low = false;
    int delta_t_prey = 0;
    int delta_t_pred = 0;
    bool prey_low = false;

    Simulation simulation{window_width, window_height, thread_pool};
    // Renderer renderer{window};

    emptyCSV("../../res/plot_info.csv");

    while (max_pop_frame > 0 && simulation.nb_pred > 1 && simulation.nb_prey > 1) {
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
            if (pred_low && simulation.nb_pred > .6 * MAX_POP_PRED) {
                score++;
                pred_low = false;
                delta_t_pred = 0;
                max_pop_frame = 1500;
            }

            if (!pred_low && simulation.nb_pred < .4 * MAX_POP_PRED) {
                score++;
                pred_low = true;
                max_pop_frame = 1500;
            }

            if (prey_low && simulation.nb_prey > .6 * MAX_POP_PREY) {
                score++;
                prey_low = false;
                delta_t_prey = 0;
                max_pop_frame = 1500;
            }

            if (!prey_low && simulation.nb_prey < .4 * MAX_POP_PREY) {
                score++;
                prey_low = true;
                max_pop_frame = 1500;
            }
            if (pred_low)
                delta_t_pred++;
            if (prey_low)
                delta_t_prey++;
            appendCSV("../../res/plot_info.csv", simulation);
            if (simulation.nb_pred >= MAX_POP_PRED * .95 || simulation.nb_prey >= MAX_POP_PREY * .95)
                max_pop_frame--;
        }
        // window.clear(sf::Color::Black);
        // renderer.render(simulation);
		// window.display();
    }

    if (simulation.nb_pred <= 1) {
        if (delta_t_pred >= 10000)
            score += .999;
        else
            score += ((float)delta_t_pred)/10000.;
    }
    if (simulation.nb_prey <= 1) {
        if (delta_t_prey >= 10000)
            score += .999;
        else
            score += ((float)delta_t_prey)/10000.;
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
