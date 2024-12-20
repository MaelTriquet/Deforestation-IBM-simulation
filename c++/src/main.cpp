#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "simulation.hpp"

int main() {

    // Create window
    constexpr int32_t window_width  = 800;
    constexpr int32_t window_height = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);


    Simulation simulation{window_width, window_height};
    Renderer renderer{window};


    while (window.isOpen()) {
        // check for user closing the window
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // update and show each frame
        simulation.update();
        window.clear(sf::Color::Black);
        renderer.render(simulation);
		window.display();
    }

}