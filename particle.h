#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>

struct Particle
{
    Particle();
    
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    
    sf::Vertex current;
    sf::Vertex previous;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
};
