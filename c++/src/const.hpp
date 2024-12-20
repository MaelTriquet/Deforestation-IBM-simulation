#pragma once
#include <cmath>

// simulation
#define MAX_POP 2000
#define PRED_START 400
#define PREY_START 400
#define TREE_START 600

// trees
#define TREES_RADIUS 10
#define FRUIT_ENERGY 50
#define FRUIT_TIME 10

// animals
#define ANIMALS_RADIUS 10
#define INITIAL_ENERGY 100
#define MUTATION_RATE .03
#define NB_RAY 5
#define RAY_LENGTH 50
#define ROT_TIME 30
#define LOST_ENERGY_REPRODUCTION 10
#define REPRODUCTION_TIMEOUT 30

// predators
#define PRED_GAIN_ENERGY_EATING 10
#define PRED_LOST_ENERGY_FIGHT 1
#define PRED_MAX_VELOCITY 6
#define PRED_MAX_RAY_ANGLE M_PI

// preys
#define INVISIBILITY_TIME 30
#define PREY_LOST_ENERGY_FIGHT 5
#define PREY_MAX_VELOCITY 5
#define PREY_MAX_RAY_ANGLE M_PI
