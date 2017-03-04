#pragma once

#include <SFML/Graphics.hpp>

struct Force
{
    Force(sf::Vector2f position, float angle = 0.f, float magnitude = 0.f);
    
    void draw(sf::RenderWindow& window);
    
    sf::Vector2f position;
    float angle;
    float magnitude;
    sf::RectangleShape line;
};
