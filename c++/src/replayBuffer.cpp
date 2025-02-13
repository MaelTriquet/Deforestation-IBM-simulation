#include "replayBuffer.hpp"
#include <random>

ReplayBuffer::ReplayBuffer(size_t capacity) : capacity(capacity) {}

void ReplayBuffer::push(torch::Tensor state, torch::Tensor action, float reward, torch::Tensor next_state, bool done) {
    if (buffer.size() >= capacity) {
        buffer.pop_front();
    }
    buffer.emplace_back(state, action, reward, next_state, done);
}

std::vector<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> ReplayBuffer::sample(size_t batch_size) {
    std::vector<std::tuple<torch::Tensor, torch::Tensor, float, torch::Tensor, bool>> batch;
    std::vector<int> indices(buffer.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);

    for (size_t i = 0; i < batch_size && i < buffer.size(); i++) {
        batch.push_back(buffer[indices[i]]);
    }
    return batch;
}

bool ReplayBuffer::is_ready(size_t batch_size) const {
    return buffer.size() >= batch_size;
}
