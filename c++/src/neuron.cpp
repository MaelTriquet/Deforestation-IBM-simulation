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

void Neuron::feedForward() {
    for (int i = 0; i < weights.size(); i++) 
        next_layer[i]->value += value * weights[i];
}