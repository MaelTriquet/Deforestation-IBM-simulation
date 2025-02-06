#include "neuron.hpp"

Neuron::Neuron(bool bias_) :
    bias(bias_)
{};

// connects to a neuron
void Neuron::add_neuron(Neuron* neuron, float weight) {
    next_layer.push_back(neuron);
    weights.push_back(weight);
}


// spread its value to the next layer according to the weights
void Neuron::feedForward(bool input) {
    // only normalise current value if hidden or output
    if (!input) 
        value = sigmoid(value);
    if (bias) 
        value = 1;
    for (int i = 0; i < weights.size(); i++) 
        next_layer[i]->value += value * weights[i];
}

// Not really sigmoid, scaled it between [-1; 1] instead of [0;1]
float Neuron::sigmoid(float x) {
    return 1 / (std::exp(-x) + 1) * 2 - 1;
}