#include <SFML/Graphics.hpp>
#include "renderer.hpp"
#include <fstream>
#include "simulation.hpp"
#include "thread_pool.hpp"
#include "actor.hpp"
#include "critic.hpp"
#include "replayBuffer.hpp"

bool appendCSV(const std::string& filename, Simulation& sim);
bool emptyCSV(const std::string& filename);
torch::Tensor compute_critic_loss(
    const std::vector<Transition>& batch,
    Critic &critic,
    Critic &critic_target,
    Actor &actor_target,
    double gamma
);

void update_actor(
    const std::vector<Transition>& batch,
    Actor &actor,
    Critic &critic,
    torch::optim::Optimizer& actor_optimizer
);

void soft_update(torch::nn::Module& target, torch::nn::Module& source, double tau);

int main() {

    std::srand(std::time(nullptr));
    long long seed = std::rand();
    Random::setSeed(seed);

    // Create window
    constexpr int32_t window_width = WINDOW_WIDTH;
    constexpr int32_t window_height = WINDOW_HEIGHT;

    tp::ThreadPool thread_pool(10);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PFE", sf::Style::Default, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);

    int max_pop_frame = 5000;
    float score = 0.;
    bool pred_low = false;
    int delta_t_prey = 0;
    int delta_t_pred = 0;
    bool prey_low = false;

    Simulation simulation{window_width, window_height, thread_pool};
    Renderer renderer{window};

    emptyCSV("../../res/plot_info.csv");

    int obs_dim = 4 + 2 * NB_RAY;
    int action_dim = 2;

    

    // Preys NN
    Actor actorPrey(obs_dim, action_dim);
    Critic criticPrey(obs_dim, action_dim);

    Actor actorPrey_target(obs_dim, action_dim);
    Critic criticPrey_target(obs_dim, action_dim);

    // Predators NN
    Actor actorPreda(obs_dim, action_dim);
    Critic criticPreda(obs_dim, action_dim);

    Actor actorPreda_target(obs_dim, action_dim);
    Critic criticPreda_target(obs_dim, action_dim);

    //Copie des réseaux dans les réseaux cibles

    {
        // On désactive la mise à jour automatique des gradients pendant la copie
        torch::NoGradGuard no_grad;
    
        // Copier les paramètres de l'acteur vers l'acteur cible
        auto actorPrey_params = actorPrey->named_parameters();
        auto actorPrey_target_params = actorPrey_target->named_parameters();
        for (auto& item : actorPrey_params) {
             actorPrey_target_params[item.key()].copy_(item.value());
        }
    
        // Copier les paramètres du critic vers le critic cible
        auto criticPrey_params = criticPrey->named_parameters();
        auto criticPrey_target_params = criticPrey_target->named_parameters();
        for (auto& item : criticPrey_params) {
             criticPrey_target_params[item.key()].copy_(item.value());
        }

        // Copier les paramètres de l'acteur vers l'acteur cible
        auto actorPreda_params = actorPreda->named_parameters();
        auto actorPreda_target_params = actorPreda_target->named_parameters();
        for (auto& item : actorPreda_params) {
                actorPreda_target_params[item.key()].copy_(item.value());
        }
    
        // Copier les paramètres du critic vers le critic cible
        auto criticPreda_params = criticPreda->named_parameters();
        auto criticPreda_target_params = criticPreda_target->named_parameters();
        for (auto& item : criticPreda_params) {
                criticPreda_target_params[item.key()].copy_(item.value());
        }
    }

    //Création d'un replay buffer
    ReplayBuffer replayBufferPrey(1000);
    ReplayBuffer replayBufferPreda(1000);

    size_t batch_size = 64;
    float tau = 0.005;
    float gamma = 0.99;

    torch::optim::Adam actorPrey_optimizer(actorPrey->parameters(), torch::optim::AdamOptions(1e-4));
    torch::optim::Adam criticPrey_optimizer(criticPrey->parameters(), torch::optim::AdamOptions(1e-4));

    torch::optim::Adam actorPreda_optimizer(actorPreda->parameters(), torch::optim::AdamOptions(1e-4));
    torch::optim::Adam criticPreda_optimizer(criticPreda->parameters(), torch::optim::AdamOptions(1e-4));

    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        for (int i = 0; i < 1; i++) {
            simulation.update(actorPrey, criticPrey, actorPreda, criticPreda, replayBufferPrey, replayBufferPreda);
            std::cout << "Buffers size :" << std::endl;
            std::cout << replayBufferPrey.size() << std::endl;
            std::cout << replayBufferPreda.size() << std::endl;

            if (replayBufferPrey.size() > batch_size) {
                auto batch = replayBufferPrey.sample(batch_size);
        
                auto loss1 = compute_critic_loss(batch, criticPrey, criticPrey_target, actorPrey_target, gamma);

                criticPrey_optimizer.zero_grad();
                loss1.backward();
                criticPrey_optimizer.step();
        
                update_actor(batch, actorPrey, criticPrey, actorPrey_optimizer);
                
                soft_update(*criticPrey_target, *criticPrey, tau);
                soft_update(*actorPrey_target, *actorPrey, tau);
            }

            if (replayBufferPreda.size() > batch_size) {
                std::cout << "Updating Preds" << std::endl;
                auto batch = replayBufferPreda.sample(batch_size);
        
                auto loss2 = compute_critic_loss(batch, criticPreda, criticPreda_target, actorPreda_target, gamma);

                criticPreda_optimizer.zero_grad();
                loss2.backward();
                criticPreda_optimizer.step();
        
                update_actor(batch, actorPreda, criticPreda, actorPreda_optimizer);
                
                soft_update(*criticPreda_target, *criticPreda, tau);
                soft_update(*actorPreda_target, *actorPreda, tau);
            }
        
        }


        window.clear(sf::Color::Black);
        renderer.render(simulation);
		window.display();
    }

    std::cout << Random::seed << std::endl;

    for (int i = 0; i < simulation.m_pop.size(); i++) {
        simulation.m_pop[i]->brain.delete_content();
    }
}

bool emptyCSV(const std::string& filename) {
    // Create an output file stream in truncate mode
    std::ofstream file(filename, std::ios::trunc);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Close the file to ensure changes are saved
    file.close();
    return true;
}


bool appendCSV(const std::string& filename, Simulation& sim) {
    // Create an output file stream in append mode
    std::ofstream file(filename, std::ios::app);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Write data to the file
    file << sim.nb_pred << "," << sim.nb_prey << "," << sim.m_pop.size() - sim.nb_pred - sim.nb_prey << "," << sim.nb_tree << "\n";

    // Close the file
    file.close();
    return true;
}


torch::Tensor compute_critic_loss(
    const std::vector<Transition>& batch,
    Critic &critic,
    Critic &critic_target,
    Actor &actor_target,
    double gamma
) {
    // Vecteurs pour accumuler les composantes du batch
    std::vector<torch::Tensor> state_vec, action_vec, reward_vec, next_state_vec, done_vec;

    // Pour chaque transition dans le batch, on récupère ses composantes
    for (const auto& transition : batch) {
        state_vec.push_back(transition.state.detach());
        action_vec.push_back(transition.action.detach());
        reward_vec.push_back(transition.reward.detach());  // Si reward est calculé via un forward
        next_state_vec.push_back(transition.next_state.detach());
        done_vec.push_back(torch::tensor({transition.done ? 1.0f : 0.0f}));
    }

    // Concaténer les tensors pour former un batch
    auto state_batch = torch::cat(state_vec, 0);       // Forme: [batch_size, state_dim]
    auto action_batch = torch::cat(action_vec, 0);       // Forme: [batch_size, action_dim]
    auto reward_batch = torch::cat(reward_vec, 0);       // Forme: [batch_size, 1]
    auto next_state_batch = torch::cat(next_state_vec, 0); // Forme: [batch_size, state_dim]
    auto done_batch = torch::cat(done_vec, 0);           // Forme: [batch_size, 1]


    if (reward_batch.dim() == 1) {
        reward_batch = reward_batch.unsqueeze(1);
    }
    if (done_batch.dim() == 1) {
        done_batch = done_batch.unsqueeze(1);
    }


    // 1. Calculer l'action suivante à partir de l'acteur cible pour l'état suivant
    auto next_actions = actor_target->forward(next_state_batch);

    // 2. Estimer la Q-value cible en passant (next_state, next_actions) dans le critic cible
    auto target_Q = critic_target->forward(next_state_batch, next_actions).detach();

    
    // 3. Calculer la cible de Bellman :
    //    y = reward + gamma * target_Q * (1 - done)
    auto y = reward_batch + gamma * target_Q * (1 - done_batch);

    // 4. Calculer la Q-value courante avec le critic pour l'état et l'action actuels
    auto current_Q = critic->forward(state_batch, action_batch);

    // 5. Calculer la loss MSE entre current_Q et la cible y
    auto loss = torch::mse_loss(current_Q, y);

    return loss;
}

void update_actor(
    const std::vector<Transition>& batch,
    Actor &actor,
    Critic &critic,
    torch::optim::Optimizer& actor_optimizer
) {
    // 1. Regrouper tous les états du mini-batch
    std::vector<torch::Tensor> state_vec;
    for (const auto& transition : batch) {
        state_vec.push_back(transition.state);
    }
    // Concaténation pour former un tenseur de forme [batch_size, state_dim]
    auto state_batch = torch::cat(state_vec, 0);

    // 2. Passage en avant (forward) par l'acteur pour obtenir les actions prédites
    auto predicted_actions = actor->forward(state_batch);

    // 3. Évaluation des Q-values pour ces actions avec le critic
    // On souhaite maximiser ces Q-values, donc la loss sera l'opposé de leur moyenne
    auto actor_loss = -critic->forward(state_batch, predicted_actions).mean();

    // 4. Rétropropagation de la loss pour mettre à jour l'acteur
    actor_optimizer.zero_grad();
    actor_loss.backward();
    actor_optimizer.step();
}


void soft_update(torch::nn::Module& target, torch::nn::Module& source, double tau) {
    // On désactive le calcul des gradients pendant la mise à jour
    torch::NoGradGuard no_grad;

    // Récupérer les paramètres nommés des deux réseaux
    auto source_params = source.named_parameters();
    auto target_params = target.named_parameters();

    // Pour chaque paramètre du réseau source, on effectue une mise à jour "soft" du paramètre cible correspondant
    for (auto& item : source_params) {
        // L'update: target = tau * source + (1 - tau) * target
        target_params[item.key()].mul_(1 - tau);
        target_params[item.key()].add_(tau * item.value());
    }
}
