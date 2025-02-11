#pragma once
#include <vector>
#include "vision.hpp"
#include "neuron.hpp"
#include "random.hpp"
#include "const.hpp"
#include "innovationHistory.hpp"

struct Vision;
class Brain {
public:
    std::vector<Neuron*> neurons;
    std::vector<Neuron*> organised_neurons;
    
    std::vector<Gene*> genes;


    int inputs, outputs, layers, last_neuron_idx;

    Brain(int inputs_, int outputs_);
    Brain(const Brain& parent1, const Brain& parent2);

    void delete_content();
    void think(const Vision& vision, float* decision);

    void mutate();

    void addConn();

    void addNeuron();

    void organiseNeurons();

    void show() const;

    int organiseLayers();

    Neuron* find_neuron_from_inno(int inno);
};