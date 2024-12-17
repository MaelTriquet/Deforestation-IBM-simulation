#pragma once
#include "simulation.hpp"
#include <SFML/Graphics.hpp>

class Renderer
{
public:
    explicit
    Renderer(sf::RenderTarget& target);

    void render(const Simulation& simulation) const;

private:
    sf::RenderTarget& m_target;
};