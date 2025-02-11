#include "neuron.hpp"

Neuron::Neuron(bool bias_, int layer_, int idx_, int inno_) :
    bias(bias_),
    layer{layer_},
    idx{idx_},
    innovation{inno_}
{};

// spread its value to the next layer according to the weights
void Neuron::feedForward(bool input) {
    // only normalise current value if hidden or output
    if (!input) 
        value = sigmoid(value);
    if (bias) 
        value = 1;
    for (Gene* g : outgoing_conns) 
        if (g->enabled)
            g->to->value += value * g->weight;
}

// Not really sigmoid, scaled it between [-1; 1] instead of [0;1]
float Neuron::sigmoid(float x) {
    return 1 / (std::exp(-x) + 1) * 2 - 1;
}

bool Neuron::connected(Neuron* to) {
    for (Gene* g : outgoing_conns)
        if (g->to == to)
            return true;
    
    return false;
}