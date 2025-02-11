#include "brain.hpp"
#include <iostream>

// creates a random brain
Brain::Brain(int inputs_, int outputs_) :
    inputs(inputs_),
    outputs(outputs_),
    layers{2},
    last_neuron_idx{0}
{
    // create necessary neurons

    for (int i = 0; i < inputs; i++)
        neurons.push_back(new Neuron(false, 0, last_neuron_idx++));
    neurons.push_back(new Neuron(true, 0, last_neuron_idx++));

    for (int j = 0; j < outputs; j++) {
        neurons.push_back(new Neuron(false, 1, last_neuron_idx++));
        Gene* bias_conn = new Gene(neurons[inputs], neurons[last_neuron_idx-1], Random::rand() * 2 - 1);
        genes.push_back(bias_conn);
        neurons[inputs]->outgoing_conns.push_back(bias_conn);
    }
   organiseNeurons();
}

Brain::Brain(const Brain& to_copy) :
    layers{to_copy.layers},
    last_neuron_idx{to_copy.last_neuron_idx},
    inputs{to_copy.inputs},
    outputs{to_copy.outputs}
{
    for (int i = 0; i < to_copy.neurons.size(); i++)
        neurons.push_back(new Neuron(to_copy.neurons[i]->bias, to_copy.neurons[i]->layer, to_copy.neurons[i]->idx));

    for (int i = 0; i < to_copy.genes.size(); i++) {
        genes.push_back(new Gene(neurons[to_copy.genes[i]->from->idx], neurons[to_copy.genes[i]->to->idx], to_copy.genes[i]->weight));
        genes[genes.size()-1]->from->outgoing_conns.push_back(genes[genes.size()-1]);
    }
    organiseNeurons();
}


// fills decision with the outcome of the feedForward from the given inputs
void Brain::think(const Vision& vision, float* decision) {
    // reset neurons values
    for (Neuron* n : neurons)
        if (!n->bias)
            n->value = 0;

    // fill input
    neurons[0]->value = vision.energy;
    neurons[1]->value = vision.fleeing;
    neurons[3]->value = vision.velocity.x;
    neurons[4]->value = vision.velocity.y;
    for (int i = 5; i < inputs; i++)
        neurons[i]->value = vision.rays[i-5];

    organiseNeurons();
    // feedForward
    for (int i = 0; i < organised_neurons.size() - outputs; i++)
        organised_neurons[i]->feedForward(i < inputs);

    // retrieve output
    for (int i = 0; i < outputs; i++)
        decision[i] = neurons[inputs + 1 + i]->value;
}

// set each weight to a random one with the prob MUTATION_RATE
void Brain::mutate() {
    if (Random::rand() < .8) {// prob mutate weight
        int idx = Random::randint(genes.size());
        if (idx < genes.size())
            genes[idx]->weight = Random::rand() * 2 - 1;
        else 
            addConn();
    }

    if (Random::rand() < .35) // prob add connection
        addConn();

    if (Random::rand() < .07) // prob add neuron
        addNeuron();
}

void Brain::addConn() {
    // if fully connected, add a neuron 
    // count max conn | REQUIRES ORAGNISED_NEURONS UP TO DATE 
    organiseNeurons();
    int max_count = 0;
    int cur_layer = 0;
    int count_in_layer = 0;
    int i = 0;
    for (i = 0; organised_neurons[i]->layer < layers-1; i++) {
        if (organised_neurons[i]->layer == cur_layer)
            count_in_layer++;
        else {
            max_count += count_in_layer * (organised_neurons.size() - i - 1);
            cur_layer++;
            count_in_layer = 1;
        }
    }
    max_count += count_in_layer * (organised_neurons.size() - i - 1);
    if (max_count == genes.size()) // can't add another connection, prevent infonite loop
        return addNeuron();
    
    Neuron* n1, *n2;
    do {
        n1 = neurons[Random::randint(neurons.size())];
        n2 = neurons[Random::randint(neurons.size())];
    } while (n1->layer >= n2->layer || n1->connected(n2));

    Gene* new_conn = new Gene(n1, n2, Random::rand() * 2 - 1);
    genes.push_back(new_conn);
    n1->outgoing_conns.push_back(new_conn);

}

void Brain::addNeuron() {
    if (genes.empty())
        return addConn();

    int gene_idx = Random::randint(genes.size());
    Gene* to_break = genes[gene_idx];

    // update layers if needed
    if (to_break->to->layer - to_break->from->layer == 1) {
        layers++;
        for (Neuron* n : neurons)
            if (n->layer >= to_break->to->layer)
                n->layer++;
    }
    
    //create new neuron
    Neuron* new_neuron = new Neuron(false, to_break->from->layer+1, last_neuron_idx++);
    neurons.push_back(new_neuron);
    //connect new neuron
    Gene* new_conn1 = new Gene(to_break->from, new_neuron, to_break->weight);
    genes.push_back(new_conn1);

    to_break->from->outgoing_conns.push_back(new_conn1);
    Gene* new_conn2 = new Gene(new_neuron, to_break->to, 1);
    genes.push_back(new_conn2);
    new_neuron->outgoing_conns.push_back(new_conn2);

    // remove this conn from neuron
    for (int i = 0; i < to_break->from->outgoing_conns.size(); i++)
        if (to_break->from->outgoing_conns[i] == to_break) {
            to_break->from->outgoing_conns.erase(to_break->from->outgoing_conns.begin() + i);
            break;
        }

    // remove conn from brain
    genes.erase(genes.begin() + gene_idx);
    delete to_break;
}

void Brain::organiseNeurons() {
    organised_neurons.clear();

    for (int i = 0; i < layers; i++) {
        for (int j = 0; j < neurons.size(); j++)
            if (neurons[j]->layer == i)
                organised_neurons.push_back(neurons[j]);
    }
}


void Brain::delete_content() {
    for (int i = genes.size()-1; i > -1; i--) {
        delete genes[i];
    }
    
    for (int i = neurons.size()-1; i > -1; i--) {
        delete neurons[i];
    }
}

void Brain::show() const {
    for (Neuron* n : organised_neurons) {
        std::cout << "Layer : " << n->layer << ", idx : " << n->idx << ", connected to :\n";
        for (Gene* g : n->outgoing_conns) {
            std::cout << g->to->idx << " ";
        }
        std::cout << "\n\n";
    }
}