#include "random.hpp"

std::random_device Random::rd;
long long Random::seed = 0;
std::mt19937 Random::gen(seed);
std::uniform_real_distribution<> Random::dis(0.0, 1.0);

void Random::setSeed(long long new_seed) {
    seed = new_seed;
    gen.seed(seed);
}

float Random::rand() {
    return dis(gen);
}

int Random::randint(int max) {
    return std::floor(dis(gen) * max);
}

int Random::randint(int min, int max) {
    return std::floor(dis(gen) * (max - min) + min);
}