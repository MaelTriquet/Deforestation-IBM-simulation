#include "neuron.hpp"

Neuron::Neuron(int index_, bool bias_) :
    index(index_), 
    bias(bias_)
{};

void Neuron::add_neuron(Neuron* neuron) {
    next_layer.push_back(neuron);
}

void Neuron::add_weight(float weight) {
    weights.push_back(weight);
}

void Neuron::feedForward(bool input) {
    if (!input) 
        value = sigmoid(value);
    if (bias) 
        value = 1;
    for (int i = 0; i < weights.size(); i++) 
        next_layer[i]->value += value * weights[i];
}

float Neuron::sigmoid(float x) {
    return 1 / (std::exp(-x) + 1) * 2 - 1;
}