#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <fstream>
#include "simulation.hpp"
#include "thread_pool.hpp"
#include "actor.hpp"
#include "critic.hpp"
#include "replayBuffer.hpp"

bool appendCSV(const std::string& filename, Simulation& sim);
bool emptyCSV(const std::string& filename);

int main() {

    std::srand(std::time(nullptr));
    long long seed = std::rand();
    Random::setSeed(seed);

    // Create window
    constexpr int32_t window_width = WINDOW_WIDTH;
    constexpr int32_t window_height = WINDOW_HEIGHT;

    tp::ThreadPool thread_pool(10);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    int max_pop_frame = 5000;
    float score = 0.;
    bool pred_low = false;
    int delta_t_prey = 0;
    int delta_t_pred = 0;
    bool prey_low = false;

    Simulation simulation{window_width, window_height, thread_pool};
    Renderer renderer{window};

    emptyCSV("../../res/plot_info.csv");

    int obs_dim = 4 + 2 * NB_RAY;
    int action_dim = 2;

    Actor actorPrey(obs_dim, action_dim);
    Critic criticPrey(obs_dim, action_dim);

    Actor actorPreda(obs_dim, action_dim);
    Critic criticPreda(obs_dim, action_dim);

    //Création d'un replay buffer
    ReplayBuffer replayBuffer(1000);

    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // update and show each frame
        for (int i = 0; i < 1; i++) {
            simulation.update(actorPrey, criticPrey, actorPreda, criticPreda, replayBuffer);
        }


        window.clear(sf::Color::Black);
        renderer.render(simulation);
		window.display();
    }

    // if (simulation.nb_pred <= 1) {
    //     if (delta_t_pred >= 10000)
    //         score += .999;
    //     else
    //         score += ((float)delta_t_pred)/10000.;
    // }
    // if (simulation.nb_prey <= 1) {
    //     if (delta_t_prey >= 10000)
    //         score += .999;
    //     else
    //         score += ((float)delta_t_prey)/10000.;
    // }

    // std::cout << score << "\n";
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
