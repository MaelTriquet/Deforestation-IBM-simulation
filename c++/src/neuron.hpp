#pragma once
#include <vector>
#include <cmath>

class Neuron {
public:

    int index;
    std::vector<Neuron*> next_layer; // neurons this is connected to
    std::vector<float> weights; // weights associated
    float value = 0; // value holder for feedForward
    bool bias;

    Neuron(int index, bool bias);

    void feedForward(bool input);
    void add_neuron(Neuron* neuron);
    void add_weight(float weight);
    float sigmoid(float x);
};