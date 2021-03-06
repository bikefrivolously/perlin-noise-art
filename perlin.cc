#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>
#include <noise/noise.h>

#include "force.h"
#include "particle.h"

#include "colour.h"
#include "util.h"

using namespace noise;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
        std::exit(2);
    }

    const std::string img_name = argv[1];
    sf::Image img;
    img.loadFromFile(img_name);

    // These values are used to compute perlin noise
    double xn = 0, yn = 0, zn = 0;

    // Some good defaults w/ cell size 10  and flow_strength 3.f
    /*const double xoffset = 0.05;
    const double yoffset = 0.05;
    const double zoffset = 0.01;*/

    // Parameters for image file creation
    std::string output_name = "output_cell10_";
    unsigned int output_index = 1;
    float output_interval = 5.f;

    // Parameters for Perlin noise generation
    const double xoffset = 0.01;
    const double yoffset = 0.01;
    const double zoffset = 0.01;

    // Parameters for particles and grid
    constexpr int cell_size = 10;
    constexpr int num_particles = 10000;
    constexpr int alpha = 10;
    constexpr float flow_strength = 3.f;

    int width = img.getSize().x;
    int height = img.getSize().y;

    const unsigned int cols = width / cell_size;
    const unsigned int rows = height / cell_size;
    width = cols * cell_size;
    height = rows * cell_size;

    // Random number generator setup
    std::random_device rd;
    std::default_random_engine gen(rd());
    // distributions for random x and y coordinates
    std::uniform_int_distribution<int> rand_x(0, width-1);
    std::uniform_int_distribution<int> rand_y(0, height-1);

    // Setup noise generator
    module::Perlin noise;
    noise.SetSeed(rand_y(gen));

    // Create a vector of vectors to hold the Force object for each square
    std::vector< std::vector<Force> > forces(rows, std::vector<Force> (cols, Force()));
    // Create a vector of vectors to hold the average colour of each square
    std::vector< std::vector<sf::Color> > colours(rows, std::vector<sf::Color> (cols, sf::Color()));

    for (unsigned int y = 0; y < rows; y++)
    {
        for (unsigned int x = 0; x < cols; x++)
        {
            forces[y][x].setPosition(x * cell_size, y * cell_size);
            forces[y][x].setMagnitude(cell_size);
            forces[y][x].setAngle(0.f);

            // Get the value of each pixel in the current grid square
            // Then find the average colour of that square
            std::vector<sf::Color> pixels;
            for (unsigned int j = y*cell_size; j < (y+1)*cell_size; j++)
            {
                for (unsigned int i = x*cell_size; i < (x+1)*cell_size; i++)
                {
                    pixels.push_back(img.getPixel(i, j));
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
    }

    sf::RenderTexture window;
    window.create(width, height);

    window.clear(sf::Color::Black);

    sf::Clock clock;
    sf::Time time;
    int frames = 0;

    sf::Clock output_clock;
    sf::Time output_time;

    while (true)
    {
        yn = 0;
        for (unsigned int y = 0; y < rows; y++)
        {
            xn = 0;
            for (unsigned int x = 0; x < cols; x++)
            {
                double n = noise.GetValue(xn, yn, zn);
                float dir = util::map(n, -1, 1, 0, 270);

                forces[y][x].setAngle(dir);

                xn += xoffset;
            }
            yn += yoffset;
        }

        sf::VertexArray lines(sf::Lines);

        for (int i = 0; i < num_particles; i++)
        {
            Particle *p = &particles[i];
            int gx, gy;
            gx = p->current.position.x / cell_size;
            gy = p->current.position.y / cell_size;
            p->current.color = colours[gy][gx];
            p->current.color.a = alpha;
            p->acceleration = forces[gy][gx].getVector() / (float)cell_size * flow_strength;
            p->update(window);
            std::array<sf::Vertex, 2> line = p->getLine();
            lines.append(line[0]);
            lines.append(line[1]);
        }

        window.draw(lines);

        //window.display();
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

        // Periodically write the image to disk
        output_time += output_clock.restart();
        if (output_time.asSeconds() >= output_interval) {
            std::cout << "Writing " << output_name << output_index << ".jpg" << std::endl;
            window.display();
            window.getTexture().copyToImage().saveToFile(output_name + std::to_string(output_index) + ".jpg");
            ++output_index;
            output_time = sf::Time();
        }

    }
}
