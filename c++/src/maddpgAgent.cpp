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
    auto actor_params = actor->parameters();
    auto target_params = target_actor->parameters();

    if (actor_params.size() != target_params.size()) {
        std::cerr << "Erreur : Nombre de paramètres différents entre actor et target_actor" << std::endl;
    } else {
        for (size_t i = 0; i < actor_params.size(); i++) {
            if (!target_params[i].defined() || !actor_params[i].defined()) {

                continue;
            }
            target_params[i].data().copy_(actor_params[i].data());

        }
    }

    auto critic_params = critic->parameters();
    auto target_critic_params = target_critic->parameters();

    if (critic_params.size() != target_critic_params.size()) {
        std::cerr << "Erreur : Nombre de paramètres différents entre critic et target_critic" << std::endl;
    } else {
        for (size_t i = 0; i < critic_params.size(); i++) {
            if (!target_critic_params[i].defined() || !critic_params[i].defined()) {

                continue;
            }
            target_critic_params[i].data().copy_(critic_params[i].data());

        }
    }


}

MADDPGAgent::MADDPGAgent(const MADDPGAgent& other)
    : actor(torch::nn::Sequential(other.actor)),
      critic(torch::nn::Sequential(other.critic)),
      target_actor(torch::nn::Sequential(other.target_actor)),
      target_critic(torch::nn::Sequential(other.target_critic)),
      actor_optimizer(actor->parameters(), torch::optim::AdamOptions(0.001)),
      critic_optimizer(critic->parameters(), torch::optim::AdamOptions(0.001)),
      gamma(other.gamma),
      tau(other.tau) {
    
    // Copie manuelle des poids
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

void MADDPGAgent::mutate() {
    for (auto& param : actor->parameters()) {
        if (param.requires_grad()) {
            torch::Tensor noise = torch::randn_like(param) * 0.1;  // Ajoute un bruit gaussien faible
            param.data().add_(noise);
        }
    }
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


