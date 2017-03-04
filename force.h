#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

constexpr double pi() { return std::atan(1) * 4; }

struct Force
{
    Force();
    Force(sf::Vector2f position, float angle = 0.f, float magnitude = 0.f);
    
    void draw(sf::RenderWindow& window);
    
    void setPosition(int x, int y);
    void setMagnitude(float magnitude);
    void setAngle(float angle);
    
    sf::Vector2f getVector();
    
    sf::Vector2f position;
    float angle;
    float magnitude;
    sf::RectangleShape line;
};
