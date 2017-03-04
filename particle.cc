#include "particle.h"

Particle::Particle()
{
}


void Particle::update(sf::RenderWindow& window)
{
    previous.position = current.position;
    previous.color = current.color;
    velocity += acceleration;
    current.position += velocity;
    acceleration *= 0.f;
    
    // Bound the position to the drawable area
    if (current.position.x >= window.getSize().x)
    {
        current.position.x -= window.getSize().x;
        previous.position.x = current.position.x;
    }
    else if (current.position.x < 0)
    {
        current.position.x += window.getSize().x;
        previous.position.x = current.position.x;
    }
    
    if (current.position.y >= window.getSize().y)
    {
        current.position.y -= window.getSize().y;
        previous.position.y = current.position.y;
    }
    else if (current.position.y < 0)
    {
        current.position.y += window.getSize().y;
        previous.position.y = current.position.y;
    }
    
    // Clip the velocity between -2.0 and 2.0
    velocity.x = std::max(-2.f, std::min(velocity.x, 2.f));
    velocity.y = std::max(-2.f, std::min(velocity.y, 2.f));
}

void Particle::draw(sf::RenderWindow& window)
{
    sf::Vertex line[2];
    line[0] = previous;
    line[1] = current;
    window.draw(line, 2, sf::Lines);
}
