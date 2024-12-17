#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include "simulation.hpp"
#include "Animal.cpp"

int main() {

    Animal animal(100, sf::Vector2f(50, 50), sf::Vector2f(1, 1), 10);

    // Create window
    constexpr int32_t window_width  = 800;
    constexpr int32_t window_height = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    Simulation simulation;
    Renderer renderer{window};


    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        simulation.update();
        window.clear(sf::Color::White);
        renderer.render(simulation);
		window.display();
    }

}