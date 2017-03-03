#include <iostream>
#include <SFML/Graphics.hpp>
#include <noise/noise.h>

using namespace noise;

double map(double val, double from_min, double from_max, double to_min, double to_max)
{
    double from_range, to_range;
    from_range = from_max - from_min;
    to_range = to_max - to_min;
    return (((val - from_min) * to_range) / from_range) + to_min;
}

int main()
{
    // Setup noise generator
    module::Perlin noise;
    double xn = 0, yn = 0, zn = 0;
    const double xoffset = 0.05;
    const double yoffset = 0.05;
    const double zoffset = 0.002;
    
    const int width = 800;
    const int height = 600;
    
    const int cell_size = 10;
    const int cols = width / cell_size;
    const int rows = height / cell_size;
    
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Perlin");
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
                int c = map(n, -1, 1, 0, 255);
                
                sf::RectangleShape r(sf::Vector2f(cell_size, cell_size));
                r.setPosition(x*cell_size, y*cell_size);
                r.setFillColor(sf::Color(c, 0, 0));
                
                window.draw(r);
                xn += xoffset;
            }
            yn += yoffset;
        }
        window.display();
        zn += zoffset;
    }
}
