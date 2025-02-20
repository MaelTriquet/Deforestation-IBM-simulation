#pragma once
#include <torch/torch.h>
#include <vector>
#include <tuple>
#include <deque>


class MADDPGAgent {
public:
    // Réseaux neuronaux
    torch::nn::Sequential actor;
    torch::nn::Sequential critic;
    torch::nn::Sequential target_actor;
    torch::nn::Sequential target_critic;

    // Optimiseurs
    torch::optim::Adam actor_optimizer;
    torch::optim::Adam critic_optimizer;

    // Hyperparamètres
    float gamma = 0.99;
    float tau = 0.01;

    // Constructeur
    MADDPGAgent(int state_dim, int action_dim, float lr_actor, float lr_critic);

    MADDPGAgent(const MADDPGAgent& other); // Par copie

    void mutate();



    // Sélection d'une action
    torch::Tensor select_action(torch::Tensor state);

    // Mise à jour des réseaux
    void update(std::deque<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> &replay_buffer);

        

private:
    void soft_update(torch::nn::Sequential& target, torch::nn::Sequential& source, float tau);
};
