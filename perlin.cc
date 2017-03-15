#include <array>
#include <vector>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <noise/noise.h>

#include "force.h"
#include "particle.h"

#include "util.h"

using namespace noise;

constexpr int cell_size = 5;

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
    
    sf::Color avgColor(std::array<sf::Color, cell_size*cell_size> pixels)
    {
        unsigned int red = 0, blue = 0, green = 0;
        for (const auto& c : pixels)
        {
            red += c.r;
            green += c.g;
            blue += c.b;
        }
        sf::Color avg(red/pixels.size(), green/pixels.size(), blue/pixels.size());
        
        return avg;
    }
}

int main()
{
    // These values are used to compute perlin noise
    double xn = 0, yn = 0, zn = 0;
    
    // Some good defaults w/ cell size 10  and flow_strength 3.f
    /*const double xoffset = 0.05;
    const double yoffset = 0.05;
    const double zoffset = 0.01;*/
    
    const double xoffset = 0.01;
    const double yoffset = 0.01;
    const double zoffset = 0.01;
    
    // Window size
    constexpr int width = 1280;
    constexpr int height = 840;
    
    constexpr int num_particles = 10000;
    constexpr int alpha = 10;    
    constexpr float flow_strength = 3.f;
    
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
    

    // Setup noise generator
    module::Perlin noise;
    noise.SetSeed(rand_y(gen));
    
    // Create and initialize the force vectors on a grid layout
    std::vector< std::vector<Force> > forces(rows, std::vector<Force> (cols, Force()));
    std::vector< std::vector<sf::Color> > colours(rows, std::vector<sf::Color> (cols, sf::Color()));
    
    const std::string img_name = "KRL-LIZARD-2.png";
    sf::Image img;
    img.loadFromFile(img_name);
    
    for (unsigned int y = 0; y < rows; y++)
    {
        for (unsigned int x = 0; x < cols; x++)
        {
            forces[y][x].setPosition(x * cell_size, y * cell_size);
            forces[y][x].setMagnitude(cell_size);
            forces[y][x].setAngle(0.f);
            
            std::array<sf::Color, cell_size*cell_size> pixels;
            int a = 0;
            for (unsigned int j = y*cell_size; j < (y+1)*cell_size; j++)
            {
                for (unsigned int i = x*cell_size; i < (x+1)*cell_size; i++)
                {
                    pixels[a] = img.getPixel(i, j);
                    a++;
                }
            }
            colours[y][x] = util::avgColor(pixels);
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
        
        // Random color for each line (with alpha)
        //particles[i].current.color = sf::Color(r*255, g*255, b*255, 10);
        
        // All black with alpha
        particles[i].current.color = sf::Color::Black;
        particles[i].current.color.a = alpha;
    }
    
    sf::RenderWindow window(sf::VideoMode(width, height), "Perlin", sf::Style::Close);
    window.setFramerateLimit(60);
    
    window.clear(sf::Color::Black);
    
    sf::Clock clock;
    sf::Time time;
    int frames = 0;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //window.clear();
        
        yn = 0;
        for (unsigned int y = 0; y < rows; y++)
        {
            xn = 0;
            for (unsigned int x = 0; x < cols; x++)
            {
                double n = noise.GetValue(xn, yn, zn);
                float dir = util::map(n, -1, 1, 0, 270);
                
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
            p->current.color = colours[gy][gx];
            p->current.color.a = 5;
            p->acceleration = forces[gy][gx].getVector() / (float)cell_size * flow_strength;
            p->update(window);
            p->draw(window);
        }
        
        window.display();
        zn += zoffset;
        
        // Calculate FPS
        frames += 1;
        time += clock.restart();
        if (time.asSeconds() > 1)
        {
            std::cout << 1.f / time.asSeconds() * frames << std::endl;
            time = sf::Time();
            frames = 0;
        }
        
    }
}
