#pragma once
#include "simulation.hpp"
#include <SFML/Graphics.hpp>

class Renderer
{
public:
    explicit
    Renderer(sf::RenderTarget& target)
        : m_target{target}
    {

    }

    void render(const Simulation& simulation) const
    {
        for (const Animal& a : simulation.m_pop) {
            sf::CircleShape shape(a.radius);
            shape.setFillColor(sf::Color(255, 0, 0));
            shape.setPosition(a.position);
            m_target.draw(shape);
        }
    }

private:
    sf::RenderTarget& m_target;
};