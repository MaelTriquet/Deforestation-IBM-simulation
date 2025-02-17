#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <fstream>
#include "simulation.hpp"
bool appendCSV(const std::string& filename, Simulation& sim);
bool emptyCSV(const std::string& filename);
int main() {

    // Create window
    constexpr int32_t window_width = WINDOW_WIDTH;
    constexpr int32_t window_height = WINDOW_HEIGHT;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);


    Simulation simulation{window_width, window_height};
    Renderer renderer{window};

    emptyCSV("../../res/plot_info.csv");


    while (window.isOpen()) {
        // check for user closing the window
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // update and show each frame
        for (int i = 0; i < 1; i++) {
            simulation.update();
            appendCSV("../../res/plot_info.csv", simulation);
        }
        window.clear(sf::Color::Black);
        renderer.render(simulation);
		window.display();
    }

    std::cout << Random::seed << "\n";

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

    int nb_prey = 0;
    int nb_pred = 0;
    int nb_dead = 0;
    for (int i = 0; i < sim.m_pop.size(); i++) {
        if (sim.m_pop[i]->is_dead){
            nb_dead++;
            continue;
        }
        if (sim.m_pop[i]->is_pred) {
            nb_pred++;
            continue;
        }
        nb_prey++;
    }

    // Write data to the file
    file << nb_pred << "," << nb_prey << "," << nb_dead << "\n";

    // Close the file
    file.close();
    return true;
}
