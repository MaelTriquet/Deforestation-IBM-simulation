#include "brain.hpp"

Brain::Brain(int inputs_, int outputs_, int nb_hidden_layer, int nb_neuron_per_layer) :
    inputs(inputs_),
    outputs(outputs_)
{
    int i = 0;
    for (; i < inputs;)
        neurons.emplace_back(i++, false);
    neurons.emplace_back(i++, true);
    for (int j = 0; j < nb_hidden_layer; j++) {
        for (int k = 0; k < nb_neuron_per_layer; k++)
            neurons.emplace_back(i++, false);
        neurons.emplace_back(i++, true);
    }

    for (int j = 0; j < outputs; j++)
        neurons.emplace_back(i++, false);
    
    int layer_start = 0;

    for (int i = 0; i < neurons.size() - outputs; i++) {
        for (int j = layer_start; j < neurons.size(); j++) {
            if (neurons[j].bias) break;
            neurons[i].add_neuron(&neurons[j]);
            neurons[i].add_weight(0);
        }

        if (neurons[i].bias) {
            layer_start = i+1;
        }
    }
    
}

void Brain::think(const Vision& vision, float* decision) {
    // ILLEGAL
    float* vision_tab = (float*) &vision;

    for (int i = 0; i < inputs; i++) {
        neurons[i].value = vision_tab[i];
    }

    for (int i = 0; i < neurons.size(); i++)
        neurons[i].feedForward();

    for (int i = 0; i < outputs; i++)
        decision[i] = neurons[neurons.size() - outputs + i].value;
}