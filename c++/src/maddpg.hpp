#pragma once
#include <torch/torch.h>
#include <vector>
#include "replayBuffer.hpp"
#include "maddpgAgent.hpp"

class MADDPG {
public:
    std::vector<MADDPGAgent> agents;
    ReplayBuffer replay_buffer;
    size_t batch_size;
    size_t update_interval;
    int step_count;

    // Constructeur
    MADDPG(int num_agents, int state_dim, int action_dim, float lr_actor, float lr_critic, size_t buffer_capacity, size_t batch_size, size_t update_interval);

    // Ajout d'une expérience au buffer
    void store_experience(torch::Tensor state, torch::Tensor action, float reward, torch::Tensor next_state, bool done);

    // Mise à jour des agents
    void update_agents();
};
