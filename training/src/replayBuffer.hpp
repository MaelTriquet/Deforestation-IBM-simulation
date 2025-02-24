#pragma once
#include "transition.hpp"
#include <vector>
#include <random>

class ReplayBuffer {
    public:
        ReplayBuffer(size_t capacity_);
        void add(const Transition& transition);
        std::vector<Transition> sample(size_t batch_size);
        size_t size() const;
    
    private:
        size_t capacity;
        size_t position;
        std::vector<Transition> buffer;
        std::default_random_engine generator;
    };