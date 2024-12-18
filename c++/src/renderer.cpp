#include "renderer.hpp"

Renderer::Renderer(sf::RenderTarget& target):
    m_target{target}
{}

void Renderer::render(const Simulation& simulation) const {
    for (const Animal* a : simulation.m_pop) {
        sf::CircleShape shape(a->radius);
        if (a->is_collisioning) {
            shape.setFillColor(sf::Color{150, 150, 0});
        } else {
            shape.setFillColor(a->color);
        }
        shape.setPosition(a->position - sf::Vector2f(RADIUS, RADIUS));
        m_target.draw(shape);
    }
    for (const Tree* t : simulation.m_trees) {
        sf::CircleShape shape(t->radius);
        shape.setFillColor(t->color);
        shape.setPosition(t->position);
        m_target.draw(shape);
    }
}
