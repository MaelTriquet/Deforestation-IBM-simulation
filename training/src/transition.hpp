#pragma once
#include <torch/torch.h>

// Define a structure for a single transition
struct Transition {
    torch::Tensor state;
    torch::Tensor action;
    torch::Tensor reward;
    torch::Tensor next_state;
    bool done;
};