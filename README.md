# perlin-noise-art

Inspired by Daniel Shiffman's Perlin Noise and Flow Field videos:

https://www.youtube.com/user/shiffman

https://www.youtube.com/watch?v=BjoM9oKOAKY

This program takes an image as input, and periodically generates a new image, which is the result of tracing particles flowing
over the original. Daniel's video explains it much better than I can.

**Note: the current version of this program will run infinitely.
Make sure to stop it or you will eventually fill your drive with the generated images!**

Requires SFML & libnoise.
Tested using sfml-2.4.2 & libnoise-1.0.0
    
![original](http://i.imgur.com/3wp8eIC.jpg)
![result](http://i.imgur.com/2MoixtO.jpg)

The above image was generated with the following parameters:

    // Parameters for image file creation
    std::string output_name = "output_cell5_alpha5_part2000";
    unsigned int output_index = 1;
    float output_interval = 5.f;

    // Parameters for Perlin noise generation
    const double xoffset = 0.01;
    const double yoffset = 0.01;
    const double zoffset = 0.01;

    // Parameters for particles and grid
    constexpr int cell_size = 5;
    constexpr int num_particles = 2000;
    constexpr int alpha = 5;
    constexpr float flow_strength = 3.f;

![original](http://i.imgur.com/KKqkHbg.jpg)
![result](http://i.imgur.com/Bo5QVSr.jpg)

The above image was generated with the following parameters:

    // Parameters for image file creation
    std::string output_name = "output_cell5_";
    unsigned int output_index = 1;
    float output_interval = 5.f;

    // Parameters for Perlin noise generation
    const double xoffset = 0.01;
    const double yoffset = 0.01;
    const double zoffset = 0.01;

    // Parameters for particles and grid
    constexpr int cell_size = 5;
    constexpr int num_particles = 5000;
    constexpr int alpha = 10;
    constexpr float flow_strength = 3.f;
