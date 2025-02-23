// random.hpp
#pragma once
#include <random>
#include <cmath>

// static class to facilitate random gen
class Random {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_real_distribution<> dis;
public:
    static long long seed;
    static void setSeed(long long new_seed);
    static float rand();
    static int randint(int max);
    static int randint(int min, int max);
};
