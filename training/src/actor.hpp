#pragma once
#include <torch/torch.h>

struct ActorImpl : torch::nn::Module {
    // Constructeur : input_dim = dimension de l'observation, output_dim = dimension de l'action
    ActorImpl(int input_dim, int output_dim)
        : fc1(register_module("fc1", torch::nn::Linear(input_dim, 128))),
          fc2(register_module("fc2", torch::nn::Linear(128, 128))),
          fc3(register_module("fc3", torch::nn::Linear(128, output_dim))) {}

    // Fonction forward qui définit la propagation avant
    torch::Tensor forward(torch::Tensor x) {
        x = torch::relu(fc1->forward(x));
        x = torch::relu(fc2->forward(x));
        x = torch::tanh(fc3->forward(x));
        return x;
    }

    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
};

TORCH_MODULE(Actor); // Macro pour créer un alias Actor pour std::shared_ptr<ActorImpl>
