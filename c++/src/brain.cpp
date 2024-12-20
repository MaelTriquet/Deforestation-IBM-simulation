#include "brain.hpp"


// creates a random brain
Brain::Brain(int inputs_, int outputs_, int nb_hidden_layer, int nb_neuron_per_layer) :
    inputs(inputs_),
    outputs(outputs_)
{
    // create necessary neurons
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

    // and then connect them all
    
    int layer_start = inputs + 1;

    for (int i = 0; i < neurons.size() - outputs; i++) {
        for (int j = layer_start; j < neurons.size(); j++) {
            if (neurons[j].bias) break;
            neurons[i].add_neuron(&neurons[j]);
            neurons[i].add_weight(Random::rand() * 2 - 1);
        }

        if (neurons[i].bias)
            layer_start = layer_start + 1 + nb_neuron_per_layer;
    }
    
}


// fills decision with the outcome of the feedForward from the given inputs
void Brain::think(const Vision& vision, float* decision) {
    // reset neurons values
    for (Neuron& n : neurons)
        if (!n.bias)
            n.value = 0;

    // fill input
    neurons[0].value = vision.energy;
    neurons[1].value = vision.fleeing;
    neurons[3].value = vision.velocity.x;
    neurons[4].value = vision.velocity.y;
    for (int i = 5; i < inputs; i++)
        neurons[i].value = vision.rays[i-5];

    // feedForward
    for (int i = 0; i < neurons.size(); i++)
        neurons[i].feedForward(i < inputs);

    // retrieve output
    for (int i = 0; i < outputs; i++)
        decision[i] = neurons[neurons.size() - outputs + i].value;
}

// set each weight to a random one with the prob MUT_RATE
void Brain::mutate() {
    for (Neuron& n : neurons)
        for (int i = 0; i < n.weights.size(); i++)
            if (Random::rand() < MUT_RATE)
                n.weights[i] = Random::rand() * 2 - 1;
}