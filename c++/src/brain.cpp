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
        neurons.push_back(new Neuron(false, 0, last_neuron_idx++, i));
    neurons.push_back(new Neuron(true, 0, last_neuron_idx++, inputs));

    for (int j = 0; j < outputs; j++) {
        neurons.push_back(new Neuron(false, 1, last_neuron_idx++, inputs + 1 + j));
        Gene* bias_conn = new Gene(neurons[inputs], neurons[last_neuron_idx-1], Random::rand() * 2 - 1, true, innovationHistory::getInnoGene(neurons[inputs]->innovation, neurons[last_neuron_idx-1]->innovation));
        genes.push_back(bias_conn);
        neurons[inputs]->outgoing_conns.push_back(bias_conn);
    }
   organiseNeurons();
}

Brain::Brain(const Brain& parent1, const Brain& parent2) :
    inputs{parent1.inputs},
    outputs{parent1.outputs},
    layers{parent1.layers},
    last_neuron_idx{parent1.last_neuron_idx}
{
    for (int i = 0; i < parent1.neurons.size(); i++)
        neurons.push_back(new Neuron(parent1.neurons[i]->bias, parent1.neurons[i]->layer, neurons.size(), parent1.neurons[i]->innovation));

    // all neurons added, now onto the connections

    for (int i = 0; i < parent1.genes.size(); i++) {
        bool setEnabled = true;
        bool added = false;
        Neuron* n1 = find_neuron_from_inno(parent1.genes[i]->from->innovation);
        Neuron* n2 = find_neuron_from_inno(parent1.genes[i]->to->innovation);
        for (int j = 0; j < parent2.genes.size() && !added; j++) {
            if (parent1.genes[i]->innovation == parent2.genes[j]->innovation) { // if both have it
                if ((!parent1.genes[i]->enabled || !parent2.genes[j]->enabled) && Random::rand() < .75)
                    setEnabled = false;
                if (Random::rand() > .5) {
                    genes.push_back(new Gene(n1, n2, parent1.genes[i]->weight, setEnabled, parent1.genes[i]->innovation));
                } else {
                    genes.push_back(new Gene(n1, n2, parent2.genes[j]->weight, setEnabled, parent2.genes[j]->innovation));
                }
                n1->outgoing_conns.push_back(genes[genes.size()-1]);
                added = true;
            }
        }
        if (added) continue;

        // disjoint or excess
        genes.push_back(new Gene(n1, n2, parent1.genes[i]->weight, setEnabled, parent1.genes[i]->innovation));
        n1->outgoing_conns.push_back(genes[genes.size()-1]);
    }

    organiseNeurons();
    // show();
}

Neuron* Brain::find_neuron_from_inno(int inno) {
    for (Neuron* n : neurons)
        if (n->innovation == inno)  
            return n;
    return 0x0;
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
    neurons[5]->value = vision.health;
    for (int i = 6; i < inputs; i++)
        neurons[i]->value = vision.rays[i-6];

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

    if (Random::rand() < .2) // prob add neuron
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

    Gene* new_conn = new Gene(n1, n2, Random::rand() * 2 - 1, true, innovationHistory::getInnoGene(n1->innovation, n2->innovation));
    genes.push_back(new_conn);
    n1->outgoing_conns.push_back(new_conn);
}

void Brain::addNeuron() {
    if (genes.empty())
        return addConn();

    int gene_idx = Random::randint(genes.size());
    while (!genes[gene_idx]->enabled)
        gene_idx = Random::randint(genes.size());
    Gene* to_break = genes[gene_idx];

    // update layers if needed
    if (to_break->to->layer - to_break->from->layer == 1) {
        layers++;
        for (Neuron* n : neurons)
            if (n->layer >= to_break->to->layer)
                n->layer++;
    }
    
    //create new neuron
    Neuron* new_neuron = new Neuron(false, to_break->from->layer+1, last_neuron_idx++, innovationHistory::getInnoNode(to_break->innovation) + inputs + 1 + outputs);
    neurons.push_back(new_neuron);
    //connect new neuron
    Gene* new_conn1 = new Gene(to_break->from, new_neuron, to_break->weight, true, innovationHistory::getInnoGene(to_break->from->innovation, new_neuron->innovation));
    genes.push_back(new_conn1);

    to_break->from->outgoing_conns.push_back(new_conn1);
    Gene* new_conn2 = new Gene(new_neuron, to_break->to, 1, true, innovationHistory::getInnoGene(new_neuron->innovation, to_break->to->innovation));
    genes.push_back(new_conn2);
    new_neuron->outgoing_conns.push_back(new_conn2);
    to_break->enabled = false;
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
        std::cout << "Layer : " << n->layer << ", idx : " << n->innovation << ", connected to :\n";
        for (Gene* g : n->outgoing_conns) {
            std::cout << g->to->innovation << " ";
        }
        std::cout << "\n\n";
    }
}