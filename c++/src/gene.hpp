#pragma once
#include "neuron.hpp"

class Neuron;
struct Gene {
public:
    Neuron* from;
    Neuron* to;
    float weight;

    Gene(Neuron* from_, Neuron* to_, float weight_) :
        from{from_},
        to{to_},
        weight{weight_}
    {}
};