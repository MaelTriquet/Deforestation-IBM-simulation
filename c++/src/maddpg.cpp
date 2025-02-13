#include "maddpg.hpp"

MADDPG::MADDPG(int num_agents, int state_dim, int action_dim, float lr_actor, float lr_critic, size_t buffer_capacity, size_t batch_size, size_t update_interval)
    : replay_buffer(buffer_capacity), batch_size(batch_size), update_interval(update_interval), step_count(0) {
    
    for (int i = 0; i < num_agents; i++) {
        agents.emplace_back(state_dim, action_dim, lr_actor, lr_critic);
    }
}

void MADDPG::store_experience(torch::Tensor state, torch::Tensor action, float reward, torch::Tensor next_state, bool done) {
    replay_buffer.push(state, action, reward, next_state, done);
    step_count++;
    if (step_count % update_interval == 0) {
        update_agents();
    }
}

void MADDPG::update_agents() {
    if (!replay_buffer.is_ready(batch_size)) return;

    auto batch = replay_buffer.sample(batch_size);

    std::vector<MADDPGAgent*> agent_pointers;
    for (auto& agent : agents) {
        agent_pointers.push_back(&agent);
    }

    for (auto& agent : agents) {
        agent.update(batch, agent_pointers);
    }
}
