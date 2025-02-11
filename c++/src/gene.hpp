#pragma once
#include "neuron.hpp"

class Neuron;
struct Gene {
public:
    Neuron* from;
    Neuron* to;
    float weight;
    bool enabled = true;
    int innovation;

    Gene(Neuron* from_, Neuron* to_, float weight_, bool enabled_, int inno_) :
        from{from_},
        to{to_},
        weight{weight_},
        enabled{enabled_},
        innovation{inno_}
    {}
};