#include <iostream>
#include <SFML/Graphics.hpp>
#include <noise/noise.h>

#include "force.h"

using namespace noise;

double map(double val, double from_min, double from_max, double to_min, double to_max)
{
    double from_range, to_range;
    from_range = from_max - from_min;
    to_range = to_max - to_min;
    double new_val = (((val - from_min) * to_range) / from_range) + to_min;
    if (new_val > to_max)
        return to_max;
    if (new_val < to_min)
        return to_min;
    return new_val;
}

int main()
{
    // Setup noise generator
    module::Perlin noise;
    
    // These values are used to compute perlin noise
    double xn = 0, yn = 0, zn = 0;
    const double xoffset = 0.05;
    const double yoffset = 0.05;
    const double zoffset = 0.002;
    
    // Window size
    const int width = 800;
    const int height = 600;
    
    const int cell_size = 10;
    const int cols = width / cell_size;
    const int rows = height / cell_size;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "Perlin");
    //window.setFramerateLimit(60);
       
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        
        yn = 0;
        for (unsigned int y = 0; y < rows; y++)
        {
            xn = 0;
            for (unsigned int x = 0; x < cols; x++)
            {
                double n = noise.GetValue(xn, yn, zn);
                float dir = map(n, -1, 1, 0, 360);
                
                Force f(sf::Vector2f(x*cell_size, y*cell_size), dir, cell_size);
                f.draw(window);
                
                xn += xoffset;
            }
            yn += yoffset;
        }
        window.display();
        zn += zoffset;
    }
}
