#include "replayBuffer.hpp"
 
// Constructor
ReplayBuffer::ReplayBuffer(size_t capacity_){
   capacity = capacity_;
   position = 0;
}

// Add a transition to the buffer
void ReplayBuffer::add(const Transition& transition) {
    if (buffer.size() < capacity) {
        buffer.push_back(transition);
    } else {
        // Overwrite the oldest transition once capacity is reached
        buffer[position] = transition;
    }
    // Circularly update the position index
    position = (position + 1) % capacity;
}

// Sample a random batch of transitions from the buffer
std::vector<Transition> ReplayBuffer::sample(size_t batch_size) {
    std::vector<Transition> batch;
    // Ensure there are enough samples in the buffer
    if (buffer.empty()) return batch;

    std::uniform_int_distribution<size_t> distribution(0, buffer.size() - 1);
    for (size_t i = 0; i < batch_size; i++) {
        size_t index = distribution(generator);
        batch.push_back(buffer[index]);
    }
    return batch;
}

// Get the current size of the buffer
size_t ReplayBuffer::size() const { 
   return buffer.size(); 
}
