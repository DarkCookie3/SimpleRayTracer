#pragma once


#include <functional>
#include <limits>
#include <random>

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility functions
float degrees_to_radians(float degrees)
{
    return degrees * pi / 180;
}

float ffmin(float a, float b)
{
    return a <= b ? a : b;
}

float ffmax(float a, float b)
{
    return a >= b ? a : b;
}

float random_float()
{
    // Returns a random real in [0, 1).
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    static std::function<float()> rand_generator =
        std::bind(distribution, generator);

    return rand_generator();
}

float random_float(float min, float max)
{
    // Returns a random real in [min, max).
    return min + (max - min) * random_float();
}