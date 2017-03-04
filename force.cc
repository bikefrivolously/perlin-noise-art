#include "force.h"

Force::Force()
{
}

Force::Force(sf::Vector2f position, float angle, float magnitude)
: position(position), angle(angle), magnitude(magnitude)
{
    line.setPosition(position);
    line.setSize(sf::Vector2f(magnitude, 1));
    line.setRotation(angle);
}

void Force::draw(sf::RenderWindow& window)
{
    window.draw(line);
}

void Force::setPosition(int x, int y)
{
    this->position = sf::Vector2f(x, y);
    line.setPosition(this->position);
}
void Force::setMagnitude(float magnitude)
{
    this->magnitude = magnitude;
    line.setSize(sf::Vector2f(magnitude, 1));
}

void Force::setAngle(float angle)
{
    this->angle = angle;
    line.setRotation(angle);
}

sf::Vector2f Force::getVector()
{
    float x = magnitude * std::sin(angle * pi() / 180.0);
    float y = magnitude * std::cos(angle * pi() / 180.0);
    return sf::Vector2f(x, y);
}

