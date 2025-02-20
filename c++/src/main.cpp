#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "simulation.hpp"
#include "maddpg.hpp"
#include <torch/torch.h>

int main() {

    std::cout << "Initialisation des variables..." << std::endl;
    constexpr int32_t window_width  = 700;
    constexpr int32_t window_height = 700;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);


    std::cout << "Initialisation de la simulation" << std::endl;
    
    Simulation simulation{window_width, window_height};
    std::cout << "Initialisation du Renderer" << std::endl;

    Renderer renderer{window};
    std::cout << "Initialisation du MADDPG" << std::endl;
    
    std::cout << "Ouverture..." << std::endl;
    
    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        for (int i = 0; i < 1; i++) {
            simulation.update();
            //calculate mean reward of pop of simulation
            float mean_reward = 0;
            for (Animal* a : simulation.m_pop) {
                mean_reward += a->reward;
            }
            mean_reward /= simulation.m_pop.size();
            std::cout << "Mean reward: " << mean_reward << std::endl;
        }
        
        // Rendu
        window.clear(sf::Color::Black);
        renderer.render(simulation);
        window.display();
    }

    return 0;
}
