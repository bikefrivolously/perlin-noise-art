#pragma once

#include <SFML/Graphics.hpp>

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

    sf::Color avgColor(std::vector<sf::Color> pixels)
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
