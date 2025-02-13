#include "maddpgAgent.hpp"

MADDPGAgent::MADDPGAgent(int state_dim, int action_dim, float lr_actor, float lr_critic)
    : actor(torch::nn::Sequential(torch::nn::Linear(state_dim, 64), torch::nn::ReLU(),
                                  torch::nn::Linear(64, 64), torch::nn::ReLU(),
                                  torch::nn::Linear(64, action_dim), torch::nn::Tanh())),
      critic(torch::nn::Sequential(torch::nn::Linear(state_dim + action_dim, 64), torch::nn::ReLU(),
                                   torch::nn::Linear(64, 64), torch::nn::ReLU(),
                                   torch::nn::Linear(64, 1))),
      target_actor(actor),
      target_critic(critic),
      actor_optimizer(actor->parameters(), torch::optim::AdamOptions(lr_actor)),
      critic_optimizer(critic->parameters(), torch::optim::AdamOptions(lr_critic))
{
    // Copie manuelle des poids pour initialiser les réseaux cibles
    for (auto& target_param : target_actor->parameters()) {
        auto& source_param = actor->parameters()[&target_param - &target_actor->parameters()[0]];
        target_param.data().copy_(source_param.data());
    }

    for (auto& target_param : target_critic->parameters()) {
        auto& source_param = critic->parameters()[&target_param - &target_critic->parameters()[0]];
        target_param.data().copy_(source_param.data());
    }
}

torch::Tensor MADDPGAgent::select_action(torch::Tensor state) {
    return actor->forward(state).detach();
}

void MADDPGAgent::update(std::vector<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> &replay_buffer,
                         const std::vector<MADDPGAgent*> &other_agents) {
    if (replay_buffer.empty()) return;
    
    // Échantillonnage d'une expérience
    auto [state, action, reward, next_state, done] = replay_buffer.back();
    replay_buffer.pop_back();
    
    // Calcul de la cible Q
    torch::Tensor target_q = reward + gamma * target_critic->forward(torch::cat({next_state, target_actor->forward(next_state)}, 1)) * (1 - done);
    torch::Tensor current_q = critic->forward(torch::cat({state, action}, 1));
    
    // Mise à jour du critique
    torch::Tensor loss_critic = torch::mse_loss(current_q, target_q.detach());
    critic_optimizer.zero_grad();
    loss_critic.backward();
    critic_optimizer.step();
    
    // Mise à jour de l'acteur
    torch::Tensor loss_actor = -critic->forward(torch::cat({state, actor->forward(state)}, 1)).mean();
    actor_optimizer.zero_grad();
    loss_actor.backward();
    actor_optimizer.step();
    
    // Mise à jour douce des réseaux cibles
    soft_update(target_actor, actor, tau);
    soft_update(target_critic, critic, tau);
}

void MADDPGAgent::soft_update(torch::nn::Sequential& target, torch::nn::Sequential& source, float tau) {
    for (auto& target_param : target->parameters()) {
        for (auto& source_param : source->parameters()) {
            target_param.data().mul_(1 - tau).add_(source_param.data(), tau);
        }
    }
}


