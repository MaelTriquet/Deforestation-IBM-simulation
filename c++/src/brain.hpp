#include <vector>
#include "animal.hpp"
#include "neuron.hpp"

class Brain {
public:
    std::vector<Neuron> neurons;

    int inputs, outputs;

    Brain(int inputs_, int outputs_, int nb_hidden_layer, int nb_neuron_per_layer);

    void think(const Vision& vision, float* decision);
};