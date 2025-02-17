#include "renderer.hpp"
#include <iostream>

Renderer::Renderer(sf::RenderTarget& target):
    m_target{target}
{}


// draws everything from the simulation
void Renderer::render(const Simulation& simulation) const {
    for (const Tree* t : simulation.m_trees) {
        if (t->is_dead) continue;
        sf::CircleShape shape(t->radius);
        shape.setFillColor(t->color);
        shape.setPosition(t->position - sf::Vector2f(t->radius, t->radius));
        m_target.draw(shape);
    }
    for (const Animal* a : simulation.m_pop) {
        if (a == simulation.m_pop[0] or a == simulation.m_pop[1]) {
            // a->brain.show();
            sf::Vector2f ray;
            for (int i = 0; i < NB_RAY; i++) {
                float theta = -a->max_ray_angle/2 + i * a->max_ray_angle / (float)(NB_RAY-1);
                float alpha;
                if (a->velocity.x * a->velocity.y == 0 && a->velocity.x + a->velocity.y == 0)
                    alpha = 0;
                else if (a->velocity.x == 0)
                    alpha = a->velocity.y < 0 ? M_PI_2 : -M_PI_2;
                else if (a->velocity.x > 0)
                    alpha = std::atan(-a->velocity.y / a->velocity.x);
                else
                    alpha = M_PI + std::atan(-a->velocity.y / a->velocity.x);
                ray = sf::Vector2f(std::cos(alpha + theta), -std::sin(alpha + theta));
                ray *= (float)RAY_LENGTH;
                sf::Vertex line[] =
                {
                    a->position,
                    a->position + ray
                };
                m_target.draw(line, 2, sf::Lines);
            }
        }
        sf::CircleShape shape(a->radius);
        if (a->invisible > 0) {
            shape.setFillColor(sf::Color::White);
        } else if (a->is_dead) {
            shape.setFillColor(sf::Color{255, 0, 255});//, (sf::Uint8)((float) a->rotting / ROT_TIME * 255)});
        } else {
            shape.setFillColor(sf::Color{a->color.r, a->color.g, a->color.b, (sf::Uint8)(a->health / (float)MAX_ENERGY * 255)});
        }
        shape.setPosition(a->position - sf::Vector2f(a->radius, a->radius));
        m_target.draw(shape);
    }
}
