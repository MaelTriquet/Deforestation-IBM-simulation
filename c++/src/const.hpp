#pragma once
#include <cmath>

// simulation
#define MAX_POP 400
#define PRED_START MAX_POP * .2f
#define PREY_START MAX_POP - PRED_START
#define TREE_START 200

// trees
#define TREES_RADIUS 10
#define FRUIT_ENERGY 100
#define FRUIT_TIME 5

// animals
#define ANIMALS_RADIUS 10
#define INITIAL_ENERGY 1000
#define MUTATION_RATE .03
#define NB_RAY 6
#define RAY_LENGTH 150
#define ROT_TIME 30
#define LOST_ENERGY_REPRODUCTION -40
#define REPRODUCTION_TIMEOUT 100

// predators
#define PRED_GAIN_ENERGY_EATING 2000
#define PRED_LOST_ENERGY_FIGHT 0
#define PRED_MAX_VELOCITY 8
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PRED_PERCENT_DAMAGE .1f

// preys
#define INVISIBILITY_TIME 30
#define PREY_LOST_ENERGY_FIGHT 5
#define PREY_MAX_VELOCITY 6
#define PREY_MAX_RAY_ANGLE M_PI*2*.8
