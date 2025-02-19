#include <cmath>
#pragma once

#define MAX_POP_PRED 341
#define MAX_POP_PRED_PERCENT 0.7752022172350445
#define MAX_POP_PREY 563
#define MAX_POP_PREY_PERCENT 0.8381485639611891
#define TREE_START 470
#define FRUIT_ENERGY 44
#define TIME_TREE_GROWTH 233.0253731806044
#define MAX_ENERGY 3915
#define ROT_TIME 990
#define REPRODUCTION_TIMEOUT 263
#define PRED_GAIN_ENERGY_EATING 194
#define PRED_LOST_ENERGY_FIGHT_BY_PREY 482
#define PRED_PASSIVE_ENERGY_LOSS 0
#define PRED_N_MIN_CHILDREN 1
#define PRED_N_MAX_CHILDREN 2
#define PREY_LOST_ENERGY_FIGHT_BY_PRED 1105
#define PREY_PASSIVE_ENERGY_LOSS 2
#define PREY_N_MIN_CHILDREN 2
#define PREY_N_MAX_CHILDREN 5
#define MAX_HEALTH 612
#define PRED_START MAX_POP_PRED * MAX_POP_PRED_PERCENT
#define PREY_START MAX_POP_PREY * MAX_POP_PREY_PERCENT
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define ANIMALS_RADIUS 9
#define MAX_TREES_RADIUS 12
#define MIN_TREES_RADIUS 2
#define INITIAL_ENERGY MAX_ENERGY * 0.8
#define NB_RAY 8
#define RAY_LENGTH 70
#define PRED_LOST_ENERGY_FIGHT 0
#define PRED_MAX_VELOCITY 7
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PREY_MAX_VELOCITY 5
#define LOST_ENERGY_REPRODUCTION 10
#define PREY_MAX_RAY_ANGLE M_PI_2
#define INVISIBILITY_TIME 1000
#define MUTATION_RATE 0.75
#define PROB_ADD_CONNECTION 0.15
#define PROB_ADD_NEURON 0.15
#define PROB_TREE_RANDOM_SPAWN 0.2
#define TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT 0.8
#define RADIUS_PERCENT_ROTTING 0.8
#define ROTTING_PER_FRAME 1
