#include "maddpg.hpp"

MADDPG::MADDPG(std::vector<Animal*>& population, size_t buffer_capacity, size_t batch_size, size_t update_interval)
    : replay_buffer(buffer_capacity), batch_size(batch_size), update_interval(update_interval), step_count(0) {

    for (Animal* animal : population) {
        if (animal->agent) {
            agents.push_back(animal->agent);
        }
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

    // Extraction des éléments du batch
    auto state = std::get<0>(batch[0]);
    auto action = std::get<1>(batch[0]);
    auto reward = std::get<2>(batch[0]);
    auto next_state = std::get<3>(batch[0]);
    auto done = std::get<4>(batch[0]);

    for (auto* agent : agents) {  // auto* pour parcourir les pointeurs correctement
        
        // Convertir gamma en tenseur de même type que reward
        torch::Tensor gamma_tensor = torch::tensor(gamma, torch::dtype(torch::kFloat32));
    
        torch::Tensor target_q = reward + gamma_tensor * agent->target_critic->forward(
            torch::cat({next_state, agent->target_actor->forward(next_state)}, 1)
        ) * (1 - done);
        torch::Tensor current_q = agent->critic->forward(
            torch::cat({state, action}, 1)
        );

    }
    
}

