#pragma once
#include <vector>

class Neuron {
public:

    int index;
    std::vector<Neuron*> next_layer;
    std::vector<float> weights;
    float value = 0;
    bool bias;

    Neuron(int index, bool bias);

    void feedForward();
    void add_neuron(Neuron* neuron);
    void add_weight(float weight);
};