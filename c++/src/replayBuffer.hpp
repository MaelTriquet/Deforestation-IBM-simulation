#pragma once
#include <torch/torch.h>
#include <deque>
#include <tuple>

class ReplayBuffer {
public:
    ReplayBuffer(size_t capacity);
    void push(torch::Tensor state, torch::Tensor action, float reward, torch::Tensor next_state, bool done);
    std::vector<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> sample(size_t batch_size);
    bool is_ready(size_t batch_size) const;

private:
    std::deque<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> buffer;
    size_t capacity;
};
