#pragma once
#include <vector>
#include <cmath>

class Neuron {
public:

    std::vector<Neuron*> next_layer; // neurons this is connected to
    std::vector<float> weights; // weights associated
    float value = 0; // value holder for feedForward
    bool bias;

    Neuron(bool bias);

    void feedForward(bool input);
    void add_neuron(Neuron* neuron, float weight);
    float sigmoid(float x);
};