#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "simulation.hpp"
#include "maddpg.hpp"
#include <torch/torch.h>

int main() {

    std::cout << "Initialisation des variables..." << std::endl;
    constexpr int32_t window_width  = 800;
    constexpr int32_t window_height = 800;
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
    
    MADDPG maddpg(
        simulation.m_pop,  // On passe la population existante
        10000,             // Capacité du replay buffer
        64,                // Taille des batchs
        10                 // Intervalle de mise à jour
    );
    

    std::cout << "Ouverture..." << std::endl;
    
    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
        
        int count = 0;
        // Mise à jour de la simulation et des agents
        for (int i = 0; i < 1; i++) {
            for (auto& agent : simulation.m_pop) {
                // std::cout << "Récupèration de l'état..." << std::endl;
                torch::Tensor state = agent->get_state();
                // std::cout << "Taille de state: " << state.sizes() << std::endl;
                // std::cout << "Action de l'animal..." << std::endl;
                torch::Tensor action = agent->agent->select_action(state);
                // std::cout << "Déplacement n°"<< count <<"..." << std::endl;
                agent->move(window_width, window_height);

                float reward = (agent->energy <= 0) ? -100 : 0;
                if (agent->is_prey && agent->is_in_tree) reward += 10;  // Manger dans un arbre
                if (agent->is_pred && agent->is_colliding) reward += 20;  // Collision avec une proie
                

                
                // Stocker l'expérience
                torch::Tensor next_state = agent->get_state();
                bool done = (agent->energy <= 0);
                maddpg.store_experience(state, action, reward, next_state, done);

                count++;
            }
            std::cout << "Mise à jour de la simulation..." << std::endl;
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
