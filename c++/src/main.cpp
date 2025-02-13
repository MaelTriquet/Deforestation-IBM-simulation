#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "simulation.hpp"
#include "maddpg.hpp"
#include <torch/torch.h>

int main() {
    // Paramètres de la fenêtre
    constexpr int32_t window_width  = 800;
    constexpr int32_t window_height = 800;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    // Initialisation de la simulation et de MADDPG
    Simulation simulation{window_width, window_height};
    Renderer renderer{window};
    MADDPG maddpg(
        simulation.m_pop.size(), // Nombre d'agents
        4 + NB_RAY * 3,         // Dimension de l'état
        2,                      // Nombre d'actions
        0.001, 0.001,            // Learning rates
        10000,                   // Capacité du replay buffer
        64,                      // Taille des batchs
        10                       // Intervalle de mise à jour
    );

    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // Mise à jour de la simulation et des agents
        for (int i = 0; i < 1; i++) {
            for (auto& agent : simulation.m_pop) {
                torch::Tensor state = agent->get_state();
                torch::Tensor action = agent->agent->select_action(state);
                agent->move(window_width, window_height);

                float reward = (agent->energy <= 0) ? -100 : 0;
                if (agent->is_prey && agent->is_in_tree) reward += 10;  // Manger dans un arbre
                if (agent->is_pred && agent->is_colliding) reward += 20;  // Collision avec une proie
                

                
                // Stocker l'expérience
                torch::Tensor next_state = agent->get_state();
                bool done = (agent->energy <= 0);
                maddpg.store_experience(state, action, reward, next_state, done);
            }
            simulation.update();
        }
        
        // Nettoyage des agents morts
        simulation.m_pop.erase(std::remove_if(
            simulation.m_pop.begin(), simulation.m_pop.end(),
            [](Animal* a) { return a->is_dead; }
        ), simulation.m_pop.end());
        
        // Rendu
        window.clear(sf::Color::Black);
        renderer.render(simulation);
        window.display();
    }

    return 0;
}
