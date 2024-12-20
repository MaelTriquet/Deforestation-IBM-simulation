#include "random.hpp"

std::random_device Random::rd;
std::mt19937 Random::gen(rd());
std::uniform_real_distribution<> Random::dis(0.0, 1.0);

float Random::rand() {
    return dis(gen);
}

int Random::randint(int max) {
    return std::floor(dis(gen) * max);
}

int Random::randint(int min, int max) {
    return std::floor(dis(gen) * (max + min) - min);
}