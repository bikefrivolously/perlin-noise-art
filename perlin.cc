#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <noise/noise.h>

#include "force.h"
#include "particle.h"

#include "util.h"

using namespace noise;

namespace util {
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
}

int main()
{
    // Setup noise generator
    module::Perlin noise;
    
    // These values are used to compute perlin noise
    double xn = 0, yn = 0, zn = 0;
    const double xoffset = 0.05;
    const double yoffset = 0.05;
    const double zoffset = 0.001;
    
    // Window size
    const int width = 800;
    const int height = 600;
    
    const int num_particles = 5000;
    
    const int cell_size = 10;
    const int cols = width / cell_size;
    const int rows = height / cell_size;
    
    // Random number generator setup
    std::random_device rd;
    std::default_random_engine gen(rd());
    // distributions for random x and y coordinates
    std::uniform_int_distribution<int> rand_x(0, width-1);
    std::uniform_int_distribution<int> rand_y(0, height-1);
    // distribution for random hue values
    std::uniform_real_distribution<float> rand_h(0.f, 360.f);
    
    
    // Create and initialize the force vectors on a grid layout
    Force forces[rows][cols];
    
    for (unsigned int y = 0; y < rows; y++)
    {
        for (unsigned int x = 0; x < cols; x++)
        {
            forces[y][x].setPosition(x * cell_size, y * cell_size);
            forces[y][x].setMagnitude(cell_size);
            forces[y][x].setAngle(0.f);
        }
    }
    
    // Create and initialize the particles
    Particle particles[num_particles];
    for (int i = 0; i < num_particles; i++)
    {
        particles[i].current.position = sf::Vector2f(rand_x(gen), rand_y(gen));
        
        float r,g,b,h,s = 1,v = 1;
        h = rand_h(gen);
        HSVtoRGB(r, g, b, h, s, v);
        
        particles[i].current.color = sf::Color(r*255, g*255, b*255, 25);
        //particles[i].velocity = sf::Vector2f(i*0.01, i*0.2);
    }
    
    sf::RenderWindow window(sf::VideoMode(width, height), "Perlin");
    window.setFramerateLimit(60);
    
    window.clear();
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        yn = 0;
        for (unsigned int y = 0; y < rows; y++)
        {
            xn = 0;
            for (unsigned int x = 0; x < cols; x++)
            {
                double n = noise.GetValue(xn, yn, zn);
                float dir = util::map(n, -1, 1, 0, 359);
                
                forces[y][x].setAngle(dir);
                //forces[y][x].draw(window);
                
                xn += xoffset;
            }
            yn += yoffset;
        }
        
        for (int i = 0; i < num_particles; i++)
        {
            Particle *p = &particles[i];
            int gx, gy;
            gx = p->current.position.x / cell_size;
            gy = p->current.position.y / cell_size;
            p->acceleration = forces[gy][gx].getVector() / (float)cell_size * 0.5f;
            p->update(window);
            p->draw(window);
        }
        
        window.display();
        zn += zoffset;
    }
}
