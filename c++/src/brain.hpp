#pragma once
#include <vector>
#include "vision.hpp"
#include "neuron.hpp"
#include "random.hpp"
#include "const.hpp"

struct Vision;
class Brain {
public:
    std::vector<Neuron*> neurons;
    std::vector<Neuron*> organised_neurons;
    
    std::vector<Gene*> genes;

    void delete_content();

    int inputs, outputs, layers, last_neuron_idx;

    Brain(int inputs_, int outputs_);
    Brain(const Brain& to_copy);

    void think(const Vision& vision, float* decision);

    void mutate();

    void addConn();

    void addNeuron();

    void organiseNeurons();
};