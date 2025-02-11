#pragma once
#include <cmath>

// simulation
#define MAX_POP 400
#define PRED_START MAX_POP * .2f
#define PREY_START MAX_POP - PRED_START
#define TREE_START 150

// trees
#define TREES_RADIUS 10
#define FRUIT_ENERGY 100
#define FRUIT_TIME 5

// animals
#define ANIMALS_RADIUS 10
#define INITIAL_ENERGY 600
#define MAX_ENERGY 800
#define MUTATION_RATE .08
#define NB_RAY 7
#define RAY_LENGTH 120
#define ROT_TIME 40
#define LOST_ENERGY_REPRODUCTION 0
#define REPRODUCTION_TIMEOUT 1000000

// predators
#define PRED_GAIN_ENERGY_EATING 300
#define PRED_LOST_ENERGY_FIGHT 30
#define PRED_MAX_VELOCITY 7
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PRED_PERCENT_DAMAGE .02f

// preys
#define INVISIBILITY_TIME 1000
#define PREY_LOST_ENERGY_FIGHT 50
#define PREY_MAX_VELOCITY 5
#define PREY_MAX_RAY_ANGLE M_PI*2*.8
