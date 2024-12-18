#include "renderer.hpp"

Renderer::Renderer(sf::RenderTarget& target):
    m_target{target}
{}

void Renderer::render(const Simulation& simulation) const {
    for (const Animal* a : simulation.m_pop) {
        sf::CircleShape shape(a->radius);
        if (a->is_colliding) {
            shape.setFillColor(sf::Color{255, 0, 255});
        } else {
            shape.setFillColor(a->color);
        }
        shape.setPosition(a->position - sf::Vector2f(a->radius, a->radius));
        m_target.draw(shape);
    }
    for (const Tree t : simulation.m_trees) {
        sf::CircleShape shape(t.radius);
        shape.setFillColor(t.color);
        shape.setPosition(t.position - sf::Vector2f(t.radius, t.radius));
        m_target.draw(shape);
    }
}
