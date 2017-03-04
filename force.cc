#include "force.h"

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
