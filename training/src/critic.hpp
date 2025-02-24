#pragma once
#include <torch/torch.h>

struct CriticImpl : torch::nn::Module {
    // Constructeur : state_dim et action_dim correspondent aux dimensions respectives
    CriticImpl(int state_dim, int action_dim)
        : fc1(register_module("fc1", torch::nn::Linear(state_dim + action_dim, 128))),
          fc2(register_module("fc2", torch::nn::Linear(128, 128))),
          fc3(register_module("fc3", torch::nn::Linear(128, 1))) {}

    // Fonction forward qui prend l'état et l'action en entrée
    torch::Tensor forward(torch::Tensor state, torch::Tensor action) {
        auto x = torch::cat({state, action}, 1);
        x = torch::relu(fc1->forward(x));
        x = torch::relu(fc2->forward(x));
        x = fc3->forward(x); // Pas d'activation finale, car on prédit une valeur Q
        return x;
    }

    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
};

TORCH_MODULE(Critic);
