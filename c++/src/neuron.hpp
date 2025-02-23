#pragma once
#include <vector>
#include <cmath>
#include "gene.hpp"

struct Gene;
class Neuron {
public:
    std::vector<Gene*> outgoing_conns; // neurons this is connected to
    float value = 0; // value holder for feedForward
    bool bias;
    int layer;
    int idx;

    Neuron(bool bias, int layer_, int idx);

    void feedForward(bool input);
    float sigmoid(float x);

    bool connected(Neuron* to);
};