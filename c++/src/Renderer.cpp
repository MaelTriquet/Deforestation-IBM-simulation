#include "renderer.hpp"

explicit
Renderer::Renderer(sf::RenderTarget& target):
    m_target{target}
{}

void Renderer::render(const Simulation& simulation) const {
    for (const Animal& a : simulation.m_pop) {
        sf::CircleShape shape(a.radius);
        shape.setFillColor(sf::Color(255, 0, 0));
        shape.setPosition(a.position);
        m_target.draw(shape);
    }
}
