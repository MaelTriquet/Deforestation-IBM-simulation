#pragma once
#include <cmath>

// simulation
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_POP_PRED 80
#define MAX_POP_PREY 80
#define PRED_START MAX_POP_PRED * .7f
#define PREY_START MAX_POP_PREY * .7f
#define TREE_START 100

// trees
#define TREES_RADIUS 10
#define FRUIT_ENERGY 100
#define FRUIT_TIME 5

// animals
#define ANIMALS_RADIUS 10
#define INITIAL_ENERGY 1000
#define MAX_ENERGY 1000
#define MUTATION_RATE .08
#define NB_RAY 8
#define RAY_LENGTH 101
#define ROT_TIME 300
#define LOST_ENERGY_REPRODUCTION 0
#define REPRODUCTION_TIMEOUT 300

// predators
#define PRED_GAIN_ENERGY_EATING 70
#define PRED_LOST_ENERGY_FIGHT 0
#define PRED_LOST_ENERGY_FIGHT_BY_PREY MAX_ENERGY/16
#define PRED_MAX_VELOCITY 7
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PRED_N_MIN_CHILDREN 1
#define PRED_N_MAX_CHILDREN 4

// preys
#define INVISIBILITY_TIME 1000
#define PREY_LOST_ENERGY_FIGHT MAX_ENERGY/8
#define PREY_MAX_VELOCITY 5
#define PREY_MAX_RAY_ANGLE M_PI*2*.7
#define PREY_N_MIN_CHILDREN 2
#define PREY_N_MAX_CHILDREN 5
